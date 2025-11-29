### Chuẩn bị
- cài cmake (nếu chưa có) và thêm C:\Program Files\CMake\bin vào PATH (https://cmake.org/download/ tải Binary distributions: Windows x64 Installer:) <br>
- máy đã chạy được freeglut và thêm C:\msys64\mingw64\bin vào PATH (nếu trước kia setup freeglut bằng MINGW64) <br>
- có g++ và mingw32-make, có thể chạy để kiểm tra: <br>

```
g++ --version 
mingw32-make --version
```

nếu chưa có mingw32-make thì mở MSYS2 MinGW 64-bit (nếu trước kia setup freeglut bằng MINGW64) và chạy: <br>

```
pacman -S mingw-w64-x86_64-make
```

<br> <br> <br> <br> <br>
<hr>

### Build file lần đầu (ĐÃ LÀM RỒI NÊN KHÔNG CẦN PHẢI LÀM LẠI PHẦN NÀY)

```
mkdir build; cd build
cmake .. -G "MinGW Makefiles"
cmake --build . --config Release
```

<br> <br> <br> <br> <br>
<hr>

### Khi cập nhật code mới và muốn build lại thì chỉ cần

```
cd build
cmake --build . --config Release
```