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
    make -j4
    make install

    cd ${EXTERNAL_DIR}
    git clone https://github.com/KhronosGroup/glslang.git
    cd glslang
    git clone https://github.com/google/googletest.git External/googletest

    cd External/googletest
    git checkout 0c400f67fcf305869c5fb113dd296eca266c9725
    cd ../..

    ./update_glslang_sources.py

    cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX="$(pwd)" .

    make -j4
    make install
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
    python3-distutils
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
    opencolorio
    glslang
    spirv-tools
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
EXTERNAL_DIR="${CASCADE_BASE_DIR}/external"

echo "Cascade directory: ${CASCADE_BASE_DIR}"
echo "External directory path: ${EXTERNAL_DIR}"

if test x$OS != x ; then
    OS=$OS
elif [[ -n `uname -a | grep arch` ]]; then
    OS=arch
elif test -f /etc/os-release; then
    OS=`grep ^ID= /etc/os-release | cut -d= -f2`
else
    OS='uname -s'
fi
echo "OS:$OS"

case "$OS" in
    arch)
        deps_arch
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

