#!/bin/bash
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        # ...
        echo "installing runtime for Linux" 
        (exec "./scripts/linux.sh")
       
elif [[ "$OSTYPE" == "darwin"* ]]; then
        # Mac OSX
        echo "installing runtime for MacOS"
        ( "./scripts/macos.sh")

elif [[ "$OSTYPE" == "cygwin" ]]; then
        # POSIX compatibility layer and Linux environment emulation for Windows
        echo "installing runtime for WSL"
        (exec "./scripts/macos.sh")

else
        echo "not supported"
fi