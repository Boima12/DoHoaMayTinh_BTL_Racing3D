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
assets/models/ - chứa mô hình 3D
assets/shaders/ - chứa shader program
assets/textures/ - chứa hình ảnh texture dùng cho mặt đường, xe, vật thể
include/camera.h - file header
include/car.h - file header
include/game.h - file header
include/input.h - file header
include/math.h - file header
include/render.h - file header
include/timer.h - file header
include/track.h - file header
include/window.h - file header
src/core/input.cpp - Bắt phím, chuột, xử lý các sự kiện nhập từ người chơi.
src/core/timer.cpp - Hệ thống đếm thời gian/frame time để cập nhật chuyển động mượt mà.
src/core/window.cpp - Khởi tạo cửa sổ FreeGLUT, cấu hình context OpenGL, xử lý resize.
src/engine/camera.cpp - Xử lý camera: góc nhìn, vị trí, hướng, di chuyển camera.
src/engine/math.cpp - Hàm toán học hỗ trợ: vector, độ dài, chuẩn hóa, tính góc, clamp,...
src/engine/render.cpp - Hàm render chung: setup lighting, vẽ mô hình, vẽ mesh, cấu hình OpenGL.
src/game/car.cpp - Xử lý xe: vị trí, vận tốc, rẽ trái/phải, vật lý đơn giản.
src/game/game.cpp - Game loop, cập nhật trạng thái game, gọi render, điều khiển cảnh.
src/game/track.cpp - Sinh đường đua, hình dạng track, checkpoint, vật cản, màu sắc.
src/main.cpp - file entry (chạy file này)
./CMakeLists.txt - Cấu hình build cho CMake, dùng để biên dịch toàn bộ project.
./README.md - file README.md 


<br> <br> <br> <br> <br>
<hr>

