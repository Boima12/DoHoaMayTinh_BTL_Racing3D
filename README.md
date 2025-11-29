### Đồ họa máy tính - Bài tập lớn - Racing3D bằng freeglut

#### Thành viên nhóm 9: <br>
- Cao Hoàng Phước Bảo - 23IT010
- Đặng Thần Cầu - 23IT025
- Nguyễn Quốc Hoàng - 23IT087 

> [! IMPORTANT]
> Đọc file SETUP_GUIDE.md để biết cách chạy

<br> <br> <br> <br> <br>
<hr>

### Các câu lệnh Github

- Tạo branch mới để làm tính năng mới trên đó <br>
```
git branch new_branch
git checkout new_branch
```

- Đẩy branch mới lên Github <br>
```
git add .
git commit -m "Thêm chức năng gì đó"
git push origin new_branch
```

- cập nhật mới nhất từ GitHub <br>
```
git checkout main
git fetch --all --prune
git pull origin main
```

<br> <br> <br> <br> <br>
<hr>

### Cấu trúc thư mục của dự án
assets/models/ - chứa mô hình 3D <br>
assets/shaders/ - chứa shader program <br>
assets/textures/ - chứa hình ảnh texture dùng cho mặt đường, xe, vật thể <br>
include/camera.h - file header <br>
include/car.h - file header <br>
include/game.h - file header <br>
include/input.h - file header <br>
include/math.h - file header <br>
include/render.h - file header <br>
include/timer.h - file header <br>
include/track.h - file header <br>
include/window.h - file header <br>
src/core/input.cpp - Bắt phím, chuột, xử lý các sự kiện nhập từ người chơi. <br>
src/core/timer.cpp - Hệ thống đếm thời gian/frame time để cập nhật chuyển động mượt mà. <br>
src/core/window.cpp - Khởi tạo cửa sổ FreeGLUT, cấu hình context OpenGL, xử lý resize. <br>
src/engine/camera.cpp - Xử lý camera: góc nhìn, vị trí, hướng, di chuyển camera. <br>
src/engine/math.cpp - Hàm toán học hỗ trợ: vector, độ dài, chuẩn hóa, tính góc, clamp,... <br>
src/engine/render.cpp - Hàm render chung: setup lighting, vẽ mô hình, vẽ mesh, cấu hình OpenGL. <br>
src/game/car.cpp - Xử lý xe: vị trí, vận tốc, rẽ trái/phải, vật lý đơn giản. <br>
src/game/game.cpp - Game loop, cập nhật trạng thái game, gọi render, điều khiển cảnh. <br>
src/game/track.cpp - Sinh đường đua, hình dạng track, checkpoint, vật cản, màu sắc. <br>
src/main.cpp - file entry (chạy file này) <br>
./CMakeLists.txt - Cấu hình build cho CMake, dùng để biên dịch toàn bộ project. <br>
./README.md - file README.md  <br>

<br> <br> <br> <br> <br>
<hr>
