#!/bin/sh 

external_and_glslang()
{
    # install OpenColorIO in external/OpenColorIO and build it
    mkdir "${EXTERNAL_DIR}"
    cd "${EXTERNAL_DIR}"

    git clone https://github.com/AcademySoftwareFoundation/OpenColorIO
    cd OpenColorIO
    OpenColorPATH=$PWD
    #git checkout tags/v2.1.0
    mkdir build
    mkdir install
    cd build
    cmake  "${OpenColorPATH}" -DCMAKE_INSTALL_PREFIX="${OpenColorPATH}/install" -DOCIO_BUILD_PYTHON=OFF -DOCIO_BUILD_APPS=OFF -DOCIO_BUILD_TESTS=OFF -DOCIO_BUILD_GPU_TESTS=OFF -DCMAKE_BUILD_TYPE=Debug
    make 
    make install

    cd ${CASCADE_BASE_DIR}
    mkdir glslang
    cd glslang
    wget https://github.com/KhronosGroup/glslang/releases/download/master-tot/glslang-master-linux-Debug.zip
    unzip glslang-master-linux-Debug.zip
    rm glslang-master-linux-Debug.zip


}

deps_ubuntu()
{
    local packages="
    libopenimageio-dev
    libgtest-dev
    libtbb-dev
    qtcreator
    build-essential
    qtbase5-dev
    qt5-qmake
    qtbase5-dev-tools
    libopenexr-dev
    cmake
    libglew-dev
    freeglut3-dev
    "
    sudo apt install -y $packages
}
deps_arch()
{
    local packages="
    openimageio
    gtest
    tbb
    qtcreator
    gcc
    make
    cmake
    qt5-base
    openxr
    glew
    freeglut        
    "
    sudo pacman -S $packages
}
deps_fedora()
{
    local packages="
    OpenImageIO-devel
    gtest-devel
    tbb-devel
    qt-creator
    qt5-qtbase-devel
    openexr-devel
    cmake
    glew-devel
    freeglut-devel
    wget
    unzip
    "
    sudo dnf install -y $packages
}

CASCADE_BASE_DIR="$PWD/.."
EXTERNAL_DIR="${CASCADE_BASE_DIR}/extenal"

echo "Cascade directory: ${CASCADE_BASE_DIR}"
echo "External directory path: ${EXTERNAL_DIR}"

if test x$OS != x ; then
    OS=$OS
elif test -f /etc/os-release; then
    OS=`grep ^ID= /etc/os-release | cut -d= -f2`
else
    OS='uname -s'
fi

case "$OS" in
    arch)
        deps_arch 
        external_and_glslang
        ;;
    fedora)
        deps_fedora
        external_and_glslang
        ;;
    ubuntu|debian)
        deps_ubuntu
        external_and_glslang
        ;;
    *)
        echo "OS not supported. You can try to specify OS. Run OS=NAME sh install-deps.sh"

esac

