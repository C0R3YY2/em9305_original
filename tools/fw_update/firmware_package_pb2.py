# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: firmware_package.proto
"""Generated protocol buffer code."""
from google.protobuf.internal import builder as _builder
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x16\x66irmware_package.proto\x12\rem_fw_package\"9\n\x0cSilicon_Info\x12\x13\n\x0bsilicon_rev\x18\x01 \x01(\r\x12\x14\n\x0csilicon_type\x18\x02 \x01(\r\"[\n\x12Target_Information\x12\x31\n\x0csilicon_info\x18\x01 \x01(\x0b\x32\x1b.em_fw_package.Silicon_Info\x12\x12\n\nproduct_id\x18\x02 \x01(\t\"\xf7\x01\n\tFW_Header\x12\x0f\n\x07hdr_ver\x18\x01 \x01(\r\x12\x0f\n\x07hdr_len\x18\x02 \x01(\r\x12\x31\n\x0csection_code\x18\x03 \x01(\x0e\x32\x1b.em_fw_package.Section_Code\x12\x15\n\rfw_start_addr\x18\x04 \x01(\r\x12\x0f\n\x07\x66w_size\x18\x05 \x01(\r\x12\x0e\n\x06\x66w_crc\x18\x06 \x01(\r\x12\x12\n\nemcore_crc\x18\x07 \x01(\r\x12\x12\n\nfw_options\x18\x08 \x01(\r\x12\x0e\n\x06\x66w_ver\x18\t \x01(\r\x12\x14\n\x0c\x66w_exec_addr\x18\n \x01(\r\x12\x0f\n\x07hdr_crc\x18\x0b \x01(\r\"$\n\x0c\x46W_Signature\x12\t\n\x01x\x18\x01 \x01(\x0c\x12\t\n\x01y\x18\x02 \x01(\x0c\"\xee\x01\n\nFW_Element\x12(\n\x06\x66w_hdr\x18\x01 \x01(\x0b\x32\x18.em_fw_package.FW_Header\x12\x12\n\nfw_hdr_raw\x18\x02 \x01(\x0c\x12\x13\n\x0b\x66w_code_raw\x18\x03 \x01(\x0c\x12\x31\n\x0c\x66w_signature\x18\x04 \x01(\x0b\x32\x1b.em_fw_package.FW_Signature\x12\x30\n\x08\x65nc_type\x18\x05 \x01(\x0e\x32\x1e.em_fw_package.Encryption_Type\x12\x18\n\x10\x63rypto_init_data\x18\x06 \x01(\x0c\x12\x0e\n\x06\x64igest\x18\x07 \x01(\x0c\"\x86\x01\n\nFW_Package\x12\x10\n\x08\x66w_count\x18\x01 \x01(\r\x12\x36\n\x0btarget_info\x18\x02 \x01(\x0b\x32!.em_fw_package.Target_Information\x12.\n\x0b\x66w_elements\x18\x03 \x03(\x0b\x32\x19.em_fw_package.FW_Element*D\n\x0f\x45ncryption_Type\x12\n\n\x06NO_ENC\x10\x00\x12\x0b\n\x07\x41\x45S_ECB\x10\x01\x12\x0b\n\x07\x41\x45S_CBC\x10\x02\x12\x0b\n\x07\x41\x45S_CTR\x10\x03*Q\n\x0cSection_Code\x12\x13\n\x0f\x46irmwareUpdater\x10\x00\x12\x0b\n\x07\x45M_Core\x10\x01\x12\x0f\n\x0b\x43ustomerApp\x10\x03\x12\x0e\n\nBootloader\x10\x04\x62\x06proto3')

_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, globals())
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'firmware_package_pb2', globals())
if _descriptor._USE_C_DESCRIPTORS == False:

  DESCRIPTOR._options = None
  _ENCRYPTION_TYPE._serialized_start=859
  _ENCRYPTION_TYPE._serialized_end=927
  _SECTION_CODE._serialized_start=929
  _SECTION_CODE._serialized_end=1010
  _SILICON_INFO._serialized_start=41
  _SILICON_INFO._serialized_end=98
  _TARGET_INFORMATION._serialized_start=100
  _TARGET_INFORMATION._serialized_end=191
  _FW_HEADER._serialized_start=194
  _FW_HEADER._serialized_end=441
  _FW_SIGNATURE._serialized_start=443
  _FW_SIGNATURE._serialized_end=479
  _FW_ELEMENT._serialized_start=482
  _FW_ELEMENT._serialized_end=720
  _FW_PACKAGE._serialized_start=723
  _FW_PACKAGE._serialized_end=857
# @@protoc_insertion_point(module_scope)
