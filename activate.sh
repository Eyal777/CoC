# activate the virtual enviroment with esp-idf's requirements

# export the location of the toolchain
export PATH="$HOME/esp/xtensa-esp32-elf/bin:$PATH"

# export the location of esp-idf
export IDF_PATH=~/dev/micro/esp-idf

# default theme of menuconfig is dark
export MENUCONFIG_COLOR=blackbg

# activate the virtual enviroment with esp-idf's requirements
if [ ! -d .esp_coc_env ]; then
    echo "Creating a fresh new enviroment!"
    virtualenv .esp_coc_env --python $(which python2)
    echo "Done"
    source .esp_coc_env/bin/activate
    echo "Installing python dependencies"
    pip install -r $IDF_PATH/requirements.txt
    echo "Done"
    echo "Created a new enviroment succesfully"
else
   source .esp_coc_env/bin/activate
fi
