import os
import warnings
import configparser
import io
import zipfile
import argparse
import shutil
from pathlib import Path
import sys
import binascii
import firmware_package_pb2 as FW_Package

from time import perf_counter_ns

from cryptography.hazmat.primitives.asymmetric import ec, utils
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.serialization import Encoding, PrivateFormat, PublicFormat, load_pem_public_key, load_pem_private_key, NoEncryption
from cryptography.exceptions import InvalidSignature
from hashlib import sha256
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
from Crypto.Util import Counter
from Crypto.Random import get_random_bytes

from intelhex import IntelHex

default_parser = argparse.ArgumentParser(
    formatter_class=argparse.RawTextHelpFormatter ,
    description="""Welcome to the firmware package generator utility tool.
""")


## Firmware Package - file format definition
## It is proposed to use bson, a binary file format derived from JSON.
## It allows simple markups to create structures inside a file.
## Internal data structure is represented with a dict.

# Add sub-commands (mode).
cmd_parsers = default_parser.add_subparsers(title='command', dest='cmd')
cmd_parsers.required = True


def encrypt(plaintext, key, mode):
    '''
    Encryption function accepting AES-ECB, AES-CBC and AES-CTR modes.
    The last encrypted block is padded to match the AES Block size (16 bytes)

    Padding uses PKC S#7 padding methods.
    Padding can be removed by after decryption without knowledge
    of the size of the plaintext.
    '''
    starttime = perf_counter_ns()
    iv = None

    if not isinstance(plaintext, bytes):
        plaintext = bytes(plaintext)
    if mode == AES.MODE_ECB:
        cipher=AES.new(key, mode)
        starttime = perf_counter_ns()
        ciphertext = cipher.encrypt(pad(plaintext, AES.block_size))
        endtime = perf_counter_ns()
    elif mode == AES.MODE_CBC:
        # Generate the Initialization Vector
        iv = get_random_bytes(AES.block_size)
        cipher=AES.new(key, mode, iv)
        starttime = perf_counter_ns()
        ciphertext = cipher.encrypt(pad(plaintext, AES.block_size))
        endtime = perf_counter_ns()
    elif mode == AES.MODE_CTR:
        iv = get_random_bytes(AES.block_size)
        iv_int = int.from_bytes(iv,'little')
        ctr = Counter.new(AES.block_size*8, initial_value = iv_int)
        # CTR requires to share the Initial Value of the Counter.
        iv = ctr['initial_value'].to_bytes(16,'big');
        cipher = AES.new(key, mode, counter = ctr)
        starttime = perf_counter_ns()
        ciphertext = cipher.encrypt(plaintext)
        endtime = perf_counter_ns()
    else:
        print("\n\nUnsupported AES MODE!")
        ciphertext = ""
    print(f"Encryption of the firmware done in {(endtime-starttime)/1000} us")

    return ciphertext, iv


def decrypt(ciphertext, key, mode, iv=None):
    '''
    Decryption function accepting AES-ECB, AES-CBC and AES-CTR modes.
    The last encrypted block is padded to match the AES Block size (16 bytes)

    Padding uses PKC S#7 padding methods.
    Padding can be removed by after decryption without knowledge
    of the size of the plaintext.
    '''

    if mode == AES.MODE_ECB:
        cipher = AES.new(key, mode)
        plaintext = unpad(cipher.decrypt(ciphertext),AES.block_size)
    elif mode == AES.MODE_CBC and iv is not None:
        cipher = AES.new(key, mode, iv)
        plaintext = unpad(cipher.decrypt(ciphertext),AES.block_size)
    elif mode == AES.MODE_CTR and iv is not None:
        iv_int = int.from_bytes(iv,'big')
        ctr = Counter.new(AES.block_size*8, initial_value = iv_int)
        cipher = AES.new(key, AES.MODE_CTR, counter = ctr)
        plaintext = cipher.decrypt(ciphertext)
    else:
        plaintext = ""
        print("\n\nUnsupported AES MODE or missing Initialization Vector")
    return plaintext


def hash_sha256(data_to_hash):
    '''
    compute and returns the digest using hash function sha256 on the data input data.

    Input parameter:
    data_to_hash: raw input data (bytarray)

    Returned values:
    digest: Hash calculated on the data_to_hash input vector (32-bytes bytearray)
    '''

    chosenHash = hashes.SHA256()
    hasher = hashes.Hash(chosenHash)
    hasher.update(data_to_hash)
    digest = hasher.finalize()
    return digest


def sign_fw(private_key_pem_file, data_to_sign):
    '''
    sign data with private key stored in private_key_pem_file.

    Input parameters:
    - private_key_pem_file: Filename of the PEM file where to load private key.
    - data_to_sign: raw data to sign (bytearray)

    Output:
    - X coodrinate of the Signature (bytearray)
    - Y coodrinate of the Signature (bytearray)
    - digest hash calculated on the data_to_sign input vector.
    '''

    with open(private_key_pem_file, "rb") as f:
        private_key = load_pem_private_key(f.read(), password=None)

    digest = hash_sha256(data_to_sign)
    chosenHash = hashes.SHA256()
    signature = private_key.sign(digest,ec.ECDSA(utils.Prehashed(chosenHash)))
    # signature = private_key.sign(data_to_sign,ec.ECDSA(chosenHash))

    print(f"Digest: {hex(int.from_bytes(digest,'big'))}")
    srs = utils.decode_dss_signature(signature)
    print(f"Signature x(r): {hex(srs[0])}")
    print(f"Signature y(s): {hex(srs[1])}")

    # return x and y coordinates of the signature as well as the digest
    return srs[0].to_bytes(32,'big'), srs[1].to_bytes(32,'big'), digest


def verify_signature(public_key, digest, signature):
    '''
    Verify the signature of an input data vector.

    Input Parameters:
    public_key: Public key (cryptography.hazmat.primitives.asymmetric.ec.EllipticCurvePublicKey).
    digest: bytearray with 32-bytes digest
    signature: signature to verify (bytes object encoded in DER format)
    sig_x: X coordinate of the signature or certificate (bytearray, big endian)
    sig_y: Y coordinate of the signature or certificate (bytearray, big endian)

    Output: 
    Returns True is the signature is valid, False otherwise.
    This function also returns the X and Y coordinates of the public key 
    in hex format (big endian)
    '''

    chosenHash = hashes.SHA256()
    pk_pn = public_key.public_numbers()

    try:
        public_key.verify(signature, digest, ec.ECDSA(utils.Prehashed(chosenHash)))
        # public_key.verify(signature, signed_data, ec.ECDSA(chosenHash))
        return True
    except InvalidSignature:
        return False


def do_generate_fw_package():
    '''
    This function generates a firmware packages based on its inupt parameters.
    It takes a list of ihex file that will be aggregated in one single
    firmware package.

    The test firmware package can be encrypted and signed.
    '''

    fw_data = bytearray()
    
    myFWPackage = FW_Package.FW_Package()
    myFWPackage.fw_count = 0
    myFWPackage.target_info.product_id = args.product_id
    myFWPackage.target_info.silicon_info.silicon_rev = args.silicon_rev
    myFWPackage.target_info.silicon_info.silicon_type = args.silicon_type

    print(f"Firmware package generation for {args.product_id} (EM{args.silicon_type:04}-DI{args.silicon_rev:02})")
    # Initialization Vector for crypto
    iv = None
    digest = 0
    if args.ihex == "None":
        print("Please include at least one ihex file to the firmware package")
        return
    else:
        # this is a real firmware package
        print("Generating firmware package (remove nvm_booloader_base if present)")
        for ihex in args.ihex:
            print(f"Processing {ihex}")
            myFWPackage.fw_count = myFWPackage.fw_count + 1
            ih = IntelHex()
            ih.fromfile(ihex, format='hex')
            ihex_start_addr = ih.segments()[0][0]

            if(ihex_start_addr < 0x00302000):
                print("NVM Booloader detected, dropped!")
                ihex_start_addr = None
                for segment in ih.segments():
                    if segment[0] >= 0x00302000:
                        ihex_start_addr = segment[0]
                        del ih[ih.segments()[0][0]:segment[0]-1]
                        ihex_striped = ihex.split('.')[0]
                        ihex_striped = f"{ihex_striped}_striped.ihex"
                        ih.tofile(ihex_striped,format='hex')
                        break

                if ihex_start_addr == None:
                    print(f"Firmware not found in {ihex}")
                    exit()

            raw_bin = ih.tobinarray(start = ihex_start_addr)

            print(f"Data Length: {len(raw_bin)} bytes")
            # Check if FW Header Version is 0
            if raw_bin[4] == 0x00:
                fw_hdr = raw_bin[0:40]
                print(f"Firmware Header Lenght: {len(fw_hdr)} bytes")
            else:
                print(f"Unsupported FW Header version ({raw_bin[4]})")
                exit()
            myFWPackageElement = myFWPackage.fw_elements.add()
            myFWPackageElement.fw_hdr_raw = bytes(fw_hdr)
            myFWPackageElement.fw_hdr.hdr_ver = fw_hdr[4]
            if fw_hdr[4] == 0:
                myFWPackageElement.fw_hdr.hdr_len = fw_hdr[5]
                myFWPackageElement.fw_hdr.section_code = fw_hdr[6] # FW_Package.Section_Code.Value("CustomerApp")
                myFWPackageElement.fw_hdr.fw_start_addr = int.from_bytes(fw_hdr[8:11],'little')
                myFWPackageElement.fw_hdr.fw_size = int.from_bytes(fw_hdr[12:15],'little')
                myFWPackageElement.fw_hdr.fw_crc = int.from_bytes(fw_hdr[16:19],'little')
                myFWPackageElement.fw_hdr.emcore_crc = int.from_bytes(fw_hdr[20:23],'little')
                myFWPackageElement.fw_hdr.fw_options = int.from_bytes(fw_hdr[24:27],'little')
                myFWPackageElement.fw_hdr.fw_ver = int.from_bytes(fw_hdr[28:29],'little')
                myFWPackageElement.fw_hdr.fw_exec_addr = int.from_bytes(fw_hdr[30:33],'little')
                myFWPackageElement.fw_hdr.hdr_crc = int.from_bytes(fw_hdr[34:37],'little')
            else:
                print("Unsupported header version")
                return()
            
            fw_bin_start_addr = myFWPackageElement.fw_hdr.fw_start_addr-ihex_start_addr
            fw_data = raw_bin[fw_bin_start_addr:fw_bin_start_addr+myFWPackageElement.fw_hdr.fw_size]
            myFWPackageElement.enc_type = FW_Package.Encryption_Type.Value(args.encrypt)
            # Sign the firmware, if requested.
            if args.signature_pem != "None":
                print("Signature process in progress")
                data_to_sign = fw_hdr[4:] # Removal of "FHDR" initial magic number
                data_to_sign.extend(fw_data)
                digest = hash_sha256(data_to_sign)
                sx, sy, digest = sign_fw(args.signature_pem, data_to_sign)
                myFWPackageElement.fw_signature.x = sx
                myFWPackageElement.fw_signature.y = sy
                myFWPackageElement.digest = digest
            else:
                print("Firmware Package not signed")
                myFWPackageElement.fw_signature.x = [0]
                myFWPackageElement.fw_signature.y = [0]
                myFWPackageElement.digest = [0]
            # Encrypt the firmware data, if requested.
            if args.encrypt == "NO_ENC":
                print("Firmware Package not encrypted")
            else:
                print("Firmware Package is encrypted")
                # Load secrete key
                with open(args.key, "r") as f:
                    secrete_key = int(f.read(),16)
                    secrete_key = secrete_key.to_bytes(16,'little')
                    print(f"secrete key loaded from {args.key}")
                if args.encrypt == "AES_ECB":
                    print("Encryption with AES_ECB")
                    fw_data, iv = encrypt(fw_data, secrete_key, AES.MODE_ECB)
                elif args.encrypt == "AES_CBC":
                    print("Encryption with AES_CBC")
                    fw_data, iv = encrypt(fw_data, secrete_key, AES.MODE_CBC)
                elif args.encrypt == "AES_CTR":
                    print("Encryption with AES_CTR")
                    fw_data, iv = encrypt(fw_data, secrete_key, AES.MODE_CTR)
            # Append the firmware data and initialization vector to the package.
            if iv is not None:
                myFWPackageElement.crypto_init_data = iv
            myFWPackageElement.fw_code_raw = bytes(fw_data)

    ## Store the data    
    with open(args.file,'wb') as f:
        f.write(myFWPackage.SerializeToString())

fpg_parser = cmd_parsers.add_parser('generate_fw_package',
    help="Firmware Package Generator")
fpg_parser.set_defaults(func=do_generate_fw_package)
fpg_parser.add_argument('-f', '--file', default="test.pack",
    type=str, help='Output file')
fpg_parser.add_argument('-s', '--signature_pem',  default="None",
    type=str, help='Private key for digital signature (None or <private_key>.pem)')
fpg_parser.add_argument('-e', '--encrypt',  default="NO_ENC",
    type=str, help='Encryption Mode (NO_ENC, AES_ECB, AES_CBC or AES_CTR)')
fpg_parser.add_argument('-k', '--key',  default="secrete_key.txt",
    type=str, help='Encryption Secrete Key file')
fpg_parser.add_argument('-i', '--ihex', nargs='*',
    type=str, help='ihex file(s) to pack in the firmware package', default="None")
fpg_parser.add_argument('-t', '--silicon_type', default=9305,
    type=int, help='silicone type')
fpg_parser.add_argument('-r', '--silicon_rev', default=5,
    type=int, help='silicone revision (e.g., 0x05 stands for DI05)')
fpg_parser.add_argument('-p', '--product_id', default="EM9305",
    type=str, help='Product Type')


def do_generate_signature_keys():
    '''
    Generate Signature Key Pair.
    The output of this command is three files:
    - private_key.pem: The key used to sign data (i.e., firmware). To keep safe and not to disclose.
    - public_key.pem: The public key to verify the digital signature
    - public_key.txt: The public key in ASCII format (X and Y coordinates) that can be used in embedded firmware for signature verification.
    '''

    # signature key
    private_key = ec.generate_private_key(ec.SECP256R1) # Equivalent to P-256
    # verifiying key
    public_key = private_key.public_key()

    # public_key.public_numbers return int values for both X and Y coordinates.
    pn = public_key.public_numbers()
    # Convert int values to big endian hex string.
    xs = hex(pn.x)
    ys = hex(pn.y)

    fname = args.file.split('.')[0]

    # Store private and public keys in PEM files and store public key in txt file for simple
    # integration in JSON files. Can be used as is.
    with open(f"private_{fname}.pem", "wb") as f:
        f.write(private_key.private_bytes(encoding = Encoding.PEM, format = PrivateFormat.TraditionalOpenSSL, encryption_algorithm = NoEncryption()))
    with open(f"public_{fname}.pem", "wb") as f:
        f.write(public_key.public_bytes(encoding = Encoding.PEM, format = PublicFormat.SubjectPublicKeyInfo))
    with open(f"public_{fname}.txt", "w") as f:
        f.write("X and Y coordinates of the P-256 public key.\n")
        f.write("The following values can be used as is in JSON file under patch_public_key_x and patch_public_key_y\n")
        f.write(f"\"patch_public_key_x\":{{\n\t\"value\": \"{xs}\"\n}},\n")
        f.write(f"\"patch_public_key_y\":{{\n\t\"value\": \"{ys}\"\n}}\n")

    print(f"ECDSA Signature Key Pair generated in private_{fname}.pem, public_{fname}.pem and public_{fname}.txt]")

gen_signature_keys_parser = cmd_parsers.add_parser('generate_signature_keys',
    help='Generate Signature Key Pair')
gen_signature_keys_parser.set_defaults(func=do_generate_signature_keys)
gen_signature_keys_parser.add_argument('-f', '--file',
    type=str, help='Key Name (file name with no extension)', default='key')


def do_generate_secrete_key():
    '''
    Generate Encryption Key and store its value in PEM file.
    '''

    # Encryption key
    secrete_key = get_random_bytes(16)
    with open(args.file, "w") as f:
        f.write(hex(int.from_bytes(secrete_key,'little')))

    print(f"AES Secrete Key generated in {args.file}")

gen_secrete_keys_parser = cmd_parsers.add_parser('generate_secrete_key',
    help='Generate Secrete Key for Encryption')
gen_secrete_keys_parser.set_defaults(func=do_generate_secrete_key)
gen_secrete_keys_parser.add_argument('-f', '--file',
    type=str, help='Output filename', default="secrete_key.txt")


def vector_to_file(filename, raw_data, bytes_per_line=10, vector_name="data", mode="a"):
    '''
    This function writes the content of the raw data input parameter in a file.
    The format of the output is a vector declared in C language. 

    Input Parameters:
    - filename: name of the output file (typically .h or .c)
    - raw_data: input data (bytearray)
    - bytes_per_line: number of bytes per line in the output file.
    - vector_name: name of the C-like declaration
    - mode: write mode ("w" or "a") to either write a new file or append the data to an existing file.
    '''

    with open(filename, mode) as f:
        n = bytes_per_line
        if len(raw_data) == 0:
            f.write(f"uint8_t {vector_name}[] = {{}};\n")
        else:
            work = [raw_data[i*n:(i+1)*n] for i in range((len(raw_data)+n-1)//n)]
            f.write(f"uint8_t {vector_name}[] =\n")
            f.write("{\n")
            for sl in work[:-1]:
                raw_data_s = ''.join(f"0x{x:02x}, " for x in sl[:-1])
                raw_data_s = raw_data_s + f"0x{sl[-1]:02x},"
                f.write("\t"+raw_data_s)
                f.write("\n")
            sl = work[-1]
            raw_data_s = ''.join(f"0x{x:02x}, " for x in sl[:-1])
            raw_data_s = raw_data_s + f"0x{sl[-1]:02x}"
            raw_data_s = raw_data_s + "\n};"
            f.write("\t"+raw_data_s + "\n\n")


def raw_data_to_string_hex_lsb_msb(raw_data, vector_name="data", filename=None, mode="a"):
    '''
    This function converts a raw data (bytearray) to an hexadecimal string.
    If the filename parameter is not None, this function also writes it content to a file. 

    Input Parameters:
    - filename: name of the output file (typically .h or .c)
    - raw_data: input data (bytearray)
    - vector_name: name of the C-like declaration
    - mode: write mode ("w" or "a") to either write a new file or append the data to an existing file.
    '''

    raw_data_int = int.from_bytes(raw_data,'big')
    raw_data_hex = hex(raw_data_int)
    if filename is not None:
        with open(filename, mode) as f:
            f.write(f"/*{raw_data_hex}*/\n")
            f.write(f"char *{vector_name} = \"{raw_data.hex()}\";\n\n")
    return raw_data_hex


def ec_point_to_hex_string_lsb_msb(raw_point_x, raw_point_y, vector_name="point", filename=None, mode="a"):
    '''
    This function converts a raw signature (to an hexadecimal string.
    If the filename parameter is not None, this function also writes it content to a file. 

    Input Parameters:
    - raw_signature: structure with x and y coordinates where x and y are represented in a bytearray (big endian)
    - vector_name: name of the C-like declaration
    - filename: name of the output file (typically .h or .c)
    - mode: write mode ("w" or "a") to either write a new file or append the data to an existing file.

    Returns:
    - hex strings of X and Y coordinates.
    '''

    sx = int.from_bytes(raw_point_x,'big')
    sy = int.from_bytes(raw_point_y,'big')

    sx_hex = hex(sx)
    sy_hex = hex(sy)

    if filename is not None:
        with open(filename, mode) as f:
            f.write(f"/*X and Y coordinates (MSB first)*/\n")
            f.write(f"/*X: {sx_hex}*/\n")
            f.write(f"/*Y: {sy_hex}*/\n")
            f.write(f"char *{vector_name}[] = {{\n")
            f.write(f"\t\"{raw_point_x.hex()}\",\n")
            f.write(f"\t\"{raw_point_y.hex()}\",\n")
            f.write(f"}};\n\n")
    return sx_hex, sy_hex;


def do_verify_fw_package():
    '''
    Verify firmware package content and generates ihex files based on its content.
    If the firmware is signed, then the signature is verified.
    '''
    myFWPackage = FW_Package.FW_Package()

    ## Load the data    
    with open(args.file,'rb') as f:
        myFWPackage.ParseFromString(f.read())
    print(f"FW Package Count: {myFWPackage.fw_count}")
    print(f"Firmware package generation for {myFWPackage.target_info.product_id} (EM{myFWPackage.target_info.silicon_info.silicon_type:04}-DI{myFWPackage.target_info.silicon_info.silicon_rev:02})")
    fw_element_count = 0

    for fw_element in myFWPackage.fw_elements:
        fw_element_count = fw_element_count + 1

        sx = int.from_bytes(fw_element.fw_signature.x,'big')
        sy = int.from_bytes(fw_element.fw_signature.y,'big')

        code_plain = bytearray()
        # Decrypt Firmware ELement before verifying the signature.
        if FW_Package.Encryption_Type.Name(fw_element.enc_type) != "NO_ENC":
            with open(args.key, "r") as f:
                secrete_key = int(f.read(),16)
                secrete_key = secrete_key.to_bytes(16,'little')
            if FW_Package.Encryption_Type.Name(fw_element.enc_type) == "AES_ECB":
                ciphertext = fw_element.fw_code_raw
                code_plain = decrypt(ciphertext, secrete_key, AES.MODE_ECB)
            elif FW_Package.Encryption_Type.Name(fw_element.enc_type) == "AES_CBC":
                ciphertext = fw_element.fw_code_raw
                code_plain = decrypt(ciphertext, secrete_key, AES.MODE_CBC, fw_element.crypto_init_data)
            elif FW_Package.Encryption_Type.Name(fw_element.enc_type) == "AES_CTR":
                ciphertext = fw_element.fw_code_raw
                code_plain = decrypt(ciphertext, secrete_key, AES.MODE_CTR, fw_element.crypto_init_data)
            else:
                print("Unknown Encryption Method")
        else:
            code_plain = fw_element.fw_code_raw

        data_to_write = bytearray(fw_element.fw_hdr_raw)
        data_to_write.extend(code_plain)
        print(f"FW Element {fw_element_count}")
        print(f"\tFW Header")
        print(f"\t\tLength: {fw_element.fw_hdr.hdr_len}")
        print(f"\t\tSection Code: {FW_Package.Section_Code.Name(fw_element.fw_hdr.section_code)}")
        print(f"\t\tFirmware Start Address: 0x{fw_element.fw_hdr.fw_start_addr:04x}")
        print(f"\t\tFirmware Size: {fw_element.fw_hdr.fw_size}")
        print(f"\t\tFirmware CRC: 0x{fw_element.fw_hdr.fw_crc:04x}")
        print(f"\t\tEM-Core CRC: 0x{fw_element.fw_hdr.emcore_crc:04x}")
        print(f"\t\tFirmware Version: {fw_element.fw_hdr.fw_ver}")
        print(f"\t\tFirmware Execute Address: 0x{fw_element.fw_hdr.fw_exec_addr:04x}")
        print(f"\t\tFirmware Header CRC: 0x{fw_element.fw_hdr.hdr_crc:04x}")
        print(f"\tFW Code:")
        print(f"\t\tEncrypted Length: {len(fw_element.fw_code_raw)}")
        print(f"\t\tDecrypted Length: {len(data_to_write) - fw_element.fw_hdr.hdr_len}")
        print(f"\tEncryption Type: {FW_Package.Encryption_Type.Name(fw_element.enc_type)}")

        data_to_verify = bytearray(data_to_write[4:])
        digest = hash_sha256(data_to_verify)
        print(f"\tFW Signature")
        print(f"\t\tX: {fw_element.fw_signature.x.hex()}")
        print(f"\t\tY: {fw_element.fw_signature.y.hex()}")
        print(f"\t\tDigest: {fw_element.digest.hex()}")

        if fw_element.crypto_init_data is not None and fw_element.crypto_init_data != 0x0:
            print(f"\tCrypto Initialization Vector: {hex(int.from_bytes(fw_element.crypto_init_data, 'big'))}")
        print(f"\n\tSignature Verification")
        print(f"\tDigest: {hex(int.from_bytes(digest,'big'))}")
        with open(args.signature_public_pem, "rb") as f:
            public_key = load_pem_public_key(f.read())
        print(f"\tPublic Key: (can be used in this for in JSON file)")
        print(f"\t\tPublic Key x: {hex(public_key.public_numbers().x)}")
        print(f"\t\tPublic Key y: {hex(public_key.public_numbers().y)}")

        signature = utils.encode_dss_signature(int.from_bytes(fw_element.fw_signature.x,'big'), int.from_bytes(fw_element.fw_signature.y,'big'))
        valid_signature = verify_signature(public_key, digest, signature)
        if valid_signature:
            print("\tSignature verified successfully")
        else:
            print("\tSignature does not match")

        
        if args.generate_test_vectors is not None:

            output_f = f"{args.file.split('.')[0]}.h"
            aeskey_name = f"{args.key.split('.')[0]}"
            pubkey_name = f"{args.signature_public_pem.split('.')[0]}"
            prefix = args.prefix

            if fw_element_count == 1: # First element
                # very first write, create a new file
                with open(output_f,'w') as f:
                    f.write(f"#define {prefix}_FW_ELEMENT_COUNT {len(myFWPackage.fw_elements)}\n\n")
                    f.write(f"#define {prefix}_HEADER_SIZE {len(fw_element.fw_hdr_raw)}\n\n")
                    f.write(f"#define {prefix}_CODE_SIZE_ENC {len(fw_element.fw_code_raw)}\n\n")
                    f.write(f"#define {prefix}_CODE_SIZE {len(code_plain)}\n\n")

                vector_to_file(output_f, secrete_key, vector_name=f"{prefix}_aeskey")
                vector_to_file(output_f, fw_element.crypto_init_data, vector_name=f"{prefix}_iv")

                vector_to_file(output_f, public_key.public_numbers().x.to_bytes(32,'big'), vector_name=f"{prefix}_pubkey_x")
                vector_to_file(output_f, public_key.public_numbers().y.to_bytes(32,'big'), vector_name=f"{prefix}_pubkey_y")
            
            vector_to_file(output_f, fw_element.fw_signature.x, vector_name=f"{prefix}_sig_x_{fw_element_count:02}")
            vector_to_file(output_f, fw_element.fw_signature.y, vector_name=f"{prefix}_sig_y_{fw_element_count:02}")
            vector_to_file(output_f, digest, vector_name=f"{prefix}_digest_{fw_element_count:02}")

            vector_to_file(output_f, fw_element.fw_hdr_raw, vector_name=f"{prefix}_header_{fw_element_count:02}")
            vector_to_file(output_f, fw_element.fw_code_raw, vector_name=f"{prefix}_code_cipher_{fw_element_count:02}")
            vector_to_file(output_f, code_plain, vector_name=f"{prefix}_code_plain_{fw_element_count:02}")
            

            # Add Corrupted data
            # Corrupted signature
            x_corrupted = bytearray(fw_element.fw_signature.x)
            x_corrupted[0] = x_corrupted[0] + 1
            y_corrupted = bytearray(fw_element.fw_signature.y)
            y_corrupted[0] = y_corrupted[0] + 1
            vector_to_file(output_f, x_corrupted, vector_name=f"{prefix}_sig_x_corrupted_{fw_element_count:02}")
            vector_to_file(output_f, y_corrupted, vector_name=f"{prefix}_sig_y_corrupted_{fw_element_count:02}")
            # Corrupted header
            fw_header_corrupted = bytearray(fw_element.fw_hdr_raw)
            fw_header_corrupted[4] = fw_header_corrupted[4]+1 # corrupt data after "FHDR"
            vector_to_file(output_f, fw_header_corrupted, vector_name=f"{prefix}_header_corrupted_{fw_element_count:02}")
            # Corrupted fw_data

            print(f"\nFile {output_f} generated\n")


verify_fw_pkg_parser = cmd_parsers.add_parser('verify_fw_package',
    help='Display firmware package content information')
verify_fw_pkg_parser.set_defaults(func=do_verify_fw_package)
verify_fw_pkg_parser.add_argument('-f', '--file',
    type=str, help='Input file')
verify_fw_pkg_parser.add_argument('-s', '--signature_public_pem',
    type=str, help='File containing the Public Key for Signature')
verify_fw_pkg_parser.add_argument('-k', '--key',  default="secrete_key.txt",
    type=str, help='Encryption Secrete Key file')
verify_fw_pkg_parser.add_argument('-g', '--generate_test_vectors',
    help='Generate Test Vector file', action='store', nargs='*')
verify_fw_pkg_parser.add_argument('-p', '--prefix',  default="prefix",
    type=str, help='Prefix for output items')


def main():
    global args
    args = default_parser.parse_args()

    # Call the 'do' function
    args.func()


if __name__ == '__main__':
    main()
