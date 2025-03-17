Set-StrictMode -Version 2.0

# invisible char because windows console is buggy
$ic = [char]0xA0

Function GetEnvVar($var){
    return [Environment]::GetEnvironmentVariable($var, [System.EnvironmentVariableTarget]::User)
}

Function SetEnvVar($var, $value){
    [Environment]::SetEnvironmentVariable($var, $value, [System.EnvironmentVariableTarget]::User)
}

Function Get-Folder($initialDirectory="")
{
    [System.Reflection.Assembly]::LoadWithPartialName("System.windows.forms")|Out-Null

    $folder = $initialDirectory

    $foldername = New-Object System.Windows.Forms.FolderBrowserDialog
    $foldername.Description = "Select Installation Folder"
    $foldername.rootfolder = "MyComputer"
    $foldername.SelectedPath = $initialDirectory

    if($foldername.ShowDialog() -eq "OK")
    {
        $folder = $foldername.SelectedPath
    }
    return $folder
}

Function Install($source, $dest, $env_var){

    if(-not (Test-Path $source)){
        Write-Host " Installation aborted: directory $source do not exist !" -BackgroundColor DarkRed
    }

    Write-Host " Copying files from $source to $dest..."

    # Xcopy $source $dest /s /i /f /Y
    Copy-Item -Path $source -Destination $dest -Recurse -Force -Confirm

    $cur_env = GetEnvVar $env_var
        
    if($cur_env -Notlike "*$dest;*"){
        Write-Host ""
        Write-Host " Installer will add : '$dest'"
        Write-Host " to your user environement variable : '$env_var'"
        Write-Host ""
        Write-Host " Updating... (this may take a while)"
        SetEnvVar $env_var "$dest;$cur_env"
        $cur_env = GetEnvVar $env_var 
        Write-Host " Updated, Your user $env_var is now:"
        Write-Host ""
        Write-Host $cur_env
        Write-Host ""
    }else{
        Write-Host "Your user $env_var already contain $dest"
    }
    
    Write-Host " > Installation Successfull" -BackgroundColor DarkGreen -n
    Write-Host "$ic"
}

Function Uninstall([string]$dest, [string]$env_var){
    
    if(-not (Test-Path $dest)){
        Write-Host "$dest do not exist !"  -BackgroundColor DarkRed
    }

    Write-Host " Uninstalling $dest..."

    $cur_env = GetEnvVar($env_var)
        
    if($cur_env -Like "*$dest;*"){
        Write-Host ""
        Write-Host " Removing $dest from '$env_var' environment variable:"
        Write-Host ""
        Write-Host $cur_env
        Write-Host ""
        Write-Host " Updating... (this may take a while)"
        $new_var = ($cur_env.Split(';') | Where-Object -FilterScript {$_ -ne $dest}) -join ';'
        SetEnvVar $env_var $new_var
        $cur_env = GetEnvVar $env_var
        Write-Host " Updated, Your user $env_var is now:"
        Write-Host ""
        Write-Host $cur_env
        Write-Host ""
        
    }

    Write-Host "Removing $dest directory..."

    Remove-Item $dest -Recurse -Force -Confirm

    Write-Host " > Uninstalled Successfully $ic" -BackgroundColor DarkGreen -n
    Write-Host "$ic"
}

$local_exe_folder = ".\blengine.dist"
$local_pyd_folder = ".\blengine.python"

$default_exe_install_folder = "$($env:LOCALAPPDATA)\BLEngine"
$default_pyd_install_folder = "$($env:LOCALAPPDATA)\BLEngine-python"

$exe_env_var = "Blengine_Exe_Install_Path"
$pyd_env_var = "Blengine_Pyd_Install_Path"

Clear-Host

Write-Host " ---------------------------------------------"
Write-Host " |   Welcome to the BLEngine setup script    |"
Write-Host " ---------------------------------------------"


$mainloop = $True

While($mainloop){
    Write-Host ""
    Write-Host " What do you want to do ?"
    Write-Host ""

    if(Test-Path $local_exe_folder){
        $blengine_path = GetEnvVar $exe_env_var
        if(($null -ne $blengine_path) -and ($blengine_path -ne '')){
            Write-Host " [1] Update or Remove BLEngine Executable"
        }else{
            Write-Host " [1] Install BLEngine Executable"
        }
    }

    if(Test-Path $local_pyd_folder){
         $blengine_path = GetEnvVar $pyd_env_var
        if(($null -ne $blengine_path) -and ($blengine_path -ne '')){
            Write-Host " [2] Update or Remove BLEngine Python Module"
        }else{
            Write-Host " [2] Install Python Module"
        }
    }
    
    Write-Host " [Q] Quit"
    Write-Host ""
    $response = read-host " Make a selection, or q to quit"
    Write-Host ""

    switch ($response)
    { 
        '1'{
            if(Test-Path $local_exe_folder){
                $blengine_path = GetEnvVar $exe_env_var
    
                Write-Host ""
                Write-Host " > BLEngine Executable Installation"  -BackgroundColor DarkBlue -n
                Write-Host $ic
    
                if(( $null -ne $blengine_path) -and ($blengine_path -ne '')){
                    Write-Host " Installation already found in $blengine_path"
                    $loop = $True
                    While($loop){
                        Write-Host ""
                        Write-Host " What do you want to do ?"
                        Write-Host " [R] Reinstall / Update"
                        Write-Host " [U] Uninstall"
                        Write-Host " [Q] Quit"
                        Write-Host ""
                        $response = read-host " Make a selection, or q to quit"
                        Write-Host ""
                        switch ($response)
                        { 
                            'r'{
                                Install $local_exe_folder $blengine_path "Path"
                                $env:Path = GetEnvVar "Path"
                                $loop = $False
                            }
                            'u'{
                                Uninstall $blengine_path "Path"
                                SetEnvVar $exe_env_var $null
                                $loop = $False
                            }
                            'q'{
                                Write-Host "Installation Aborted." 
                                $loop = $False
                            }
                            default {
                                Write-Host "Wrong Input"
                            }
                        }
                    }
                }else{
                    $blengine_path = $default_exe_install_folder
                    Write-Host " Install the BLEngine executable to provide system-wide access to blengine CLI"
                    Write-Host ""
                    Write-Host " Default installation path is: $blengine_path"

                    $response = read-host "Press c to change it, any other key to continue "
                    if($response -eq "c"){
                        $blengine_path = Get-Folder $blengine_path
                    }
                    Write-Host ""
                    Write-Host " BLEngine will be installed in the following directory"
                    Write-Host ""
                    Write-Host $blengine_path
                    Write-Host ""

                    $response = read-host "Press a to abort, any other key to continue "

                    if($response -ne "a"){
                        Install $local_exe_folder $blengine_path "Path"
                        $env:Path = GetEnvVar "Path"
                        SetEnvVar $exe_env_var $blengine_path
                        Write-Host " You can now call blengine.exe from anywhere."
                    }else{
                        Write-Host " Installation Aborted."
                    }
                }
            }
        }
        '2'{
            if(Test-Path $local_pyd_folder){
                $blengine_path = GetEnvVar $pyd_env_var

                Write-Host ""
                Write-Host " > BLEngine Python Module Installation" -BackgroundColor DarkBlue -n
                Write-Host $ic

                if(($null -ne $blengine_path) -and ($blengine_path -ne '')){
                    Write-Host " Installation already found in  $blengine_path"
                    $loop = $True
                    While($loop){
                        Write-Host ""
                        Write-Host " What do you want to do ?"
                        Write-Host " [R] Reinstall / Update"
                        Write-Host " [U] Uninstall"
                        Write-Host " [Q] Quit"
                        Write-Host ""
                        $response = read-host " Make a selection, or q to quit"
                        Write-Host ""
                        switch ($response)
                        { 
                            'r'{
                                Install $local_pyd_folder $blengine_path "PYTHONPATH"
                                $env:PYTHONPATH = GetEnvVar "PYTHONPATH"
                                $loop = $False
                            }
                            'u'{
                                Uninstall $blengine_path "PYTHONPATH"
                                SetEnvVar $pyd_env_var $null
                                $loop = $False
                            }
                            'q'{
                                Write-Host " Installation Aborted."
                                $loop = $False
                            }
                            default {
                                Write-Host " Wrong input"
                            }
                        }
                    }
                }else{
                    $blengine_path = $default_pyd_install_folder
                    Write-Host " Install the BLEngine python module to provide system-wide "
                    Write-Host " access to blengine python package that can be used in python script"
                    Write-Host ""
                    Write-Host " Default installation path is: $blengine_path"

                    $response = read-host "Press c to change it, any other key to continue "
                    if($response -eq "c"){
                        $blengine_path = Get-Folder $blengine_path
                    }

                    Write-Host ""
                    Write-Host " BLEngine will be installed in the following directory"
                    Write-Host ""
                    Write-Host $blengine_path
                    Write-Host ""

                    $response = read-host " Press a to abort, any other key to continue "

                    if($response -ne "a"){
                        Install $local_pyd_folder $blengine_path "PYTHONPATH"
                        SetEnvVar $pyd_env_var $blengine_path
                        $env:PYTHONPATH = GetEnvVar "PYTHONPATH"
                        Write-Host " You can now call 'import blengine' in python script"
                    }else{
                        Write-Host " Installation Aborted."
                    }
                }
            }
        }
        'q'{
            Write-Host "Bye." 
            $mainloop = $False
        }
        default {
            Write-Host "Wrong Input"
        }
    }
}

Pause