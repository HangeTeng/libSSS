# libSSS

This code is implemented with reference to the work done by dujiajun in the [PSU repository](https://github.com/dujiajun/PSU).

## Tested Environment

This code and following instruction is construct on Ubuntu 20.04, with g++ 9.4 and CMake 3.24.

## Install Project and Dependencies
Download the project code and the dependent libraries libOTe and cryptoTools.

```shell
git clone https://github.com/HangeTeng/libSSS.git
cd libSSS

git clone https://github.com/osu-crypto/libOTe
cd libOTe
git checkout 3a40823f0507710193d5b90e6917878853a2f836

git clone https://github.com/ladnir/cryptoTools
cd cryptoTools
git checkout 4a83de286d05669678364173f9fdfe45a44ddbc6
```



## Compile libOTe

Compile the dependent libraries libOTe and cryptotools as dynamic link libraries.

Switch the compilation option for the libOTe and cryptotools libraries from static (```STATIC```) to dynamic (```SHARED```).

```
# in libSSS/libOTe/libOTe/CMakeLists.txt line 7
add_library(libOTe SHARED ${SRCS})

# in libSSS/libOTe/libOTe_Tests/CMakeLists.txt line 5
add_library(libOTe_Tests SHARED ${SRCS})

# in libSSS/libOTe/cryptoTools/cryptoTools/CMakeLists.txt line 9
add_library(cryptoTools SHARED ${SRCS})

# in libSSS/libOTe/cryptoTools/tests_cryptoTools/CMakeLists.txt line 5
add_library(tests_cryptoTools SHARED ${SRCS})
```

Compile the dependencies, Boost and RELIC libraries.

```shell
# in libSSS/libOTe
python build.py --setup --boost --relic
```
PS1: If the command remains stuck at "extracting boost," you can extract the Boost package in the "\libSSS\libOTe\cryptoTools\thirdparty" directory and rename it to "boost." Then, run the command with the "--boost" option again.

PS2: If you encounter a data structure error related to the black2 algorithm while compiling RELIC, 
you may find [this issue](https://github.com/Raptor3um/raptoreum/issues/48) helpful.

Choose a place to store compiled headers and static libraries, denoted by `out/install/linux`. If empty, it will be installed in `/usr/local`.

```shell
# in libSSS/libOTe
python build.py --install=out/install/linux -- -D ENABLE_RELIC=ON -D ENABLE_NP=ON -D ENABLE_KOS=ON -D ENABLE_IKNP=ON -D ENABLE_SILENTOT=ON
```


Add the dependency paths to /etc/ld.so.conf and update the cache.

```shell
# in libSSS dir
PATH_TO_PROJECT=`pwd`
echo -e "$PATH_TO_PROJECT/libOTe/cryptoTools/thirdparty/unix/lib\n$PATH_TO_PROJECT/libOTe/out/install/linux/lib" > /etc/ld.so.conf.d/libSSS.conf
ldconfig
```
If you change the install dir, do not forget to modify relevant install paths.

## Compile libSSS

```shell
# in libSSS dir
mkdir build
cd build
cmake ..
make -j
```

## Use in python
### Path.append
In the 'build' directory, you'll find an 'SSS.cpython(omitted).so' file. You can access the Python library 'SSS' by simply using 'sys.path.append' and 'import'. Here's an example of how to do it, and you can find the test code in the 'python' folder.

```python
sys.path.append(r'../build/')
import SSS
```
### Global
If you wish to use a Python package globally, use the 'cp' command to copy the .so file to the library directory. 
```shell
# in libSSS/build dir
cp $(pwd)/SSS.cpython-310-x86_64-linux-gnu.so /usr/lib/python3/dist-packages
```
If you're not sure about the library directory, you can check Python's global module search path to ensure the module can be found:
```shell
python -c "import sys; print(sys.path)"
```