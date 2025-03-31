1.Cách Tải Game


2.Cách Chơi
Chọn Start Game để bắt đầu
Sonic sẽ tự động chạy về phía trước, và nhiệm vụ của bạn là điều khiển anh ấy để né tránh chướng ngại vật.
Chọn Màn Chơi
Game có 2 màn chơi với bối cảnh bầu trời khác nhau:
Màn 1: Bầu Trời Xanh 
Màn 2: Bầu Trời Hoàng Hôn 

2.Các Thành Phần Trong Game
Sonic (Nhân Vật Chính): Chú nhím Sonic né tránh chướng ngại vật và thu thập vật phẩm.
Chướng ngại vật (Obstacles): Các chướng ngại vật xuất hiện ngẫu nhiên, cản đường Sonic.
Điểm Số (Score): Hiển thị ở góc trên cùng của màn hình, tăng dần theo thời gian .

3.Bố cục game cơ bản:
Sonic chạy từ trái sang phải trên một bầu trời cuộn vô tận. Chướng ngại vật xuất hiện ngẫu nhiên từ bên phải, di chuyển về phía Sonic.

4.Cách Chơi
Mục tiêu: Điều khiển Sonic để né tránh máy bay địch, thu thập điểm số và chạy càng xa càng tốt.
Điều khiển:
Di chuyển: Sử dụng phím space để nhảy.

Tăng tốc: Thu thập vật phẩm tăng tốc để bay nhanh hơn trong thời gian ngắn.
Khiên bảo vệ: Bảo vệ Sonic khỏi một lần va chạm.
Chướng Ngại Vật và Kẻ Thù
Game có 2 loại chướng ngại vật

Tên Chướng Ngại Vật	Chức Năng
Chướng ngại vật bay:
Chướng ngại vật di chuyển lên xuống:
Chướng ngại vật hồ nước:
Chướng ngại vật cao:
Chiến thắng: Không có điểm kết thúc (endless runner). Mục tiêu là đạt điểm số cao nhất có thể.
Thất bại: Nếu Sonic va chạm với chướng ngại vật, game sẽ kết thúc. Điểm số của bạn sẽ được hiển thị, và bạn có thể chơi lại.

Về Đồ Họa Của Game
Đồ họa của game được thiết kế đơn giản nhưng bắt mắt:
Sonic: Sử dụng texture sonic.png, với hoạt ảnh di chuyển mượt mà.
Nền: Một bầu trời cuộn vô tận với các đám mây (có thể được thêm trong tương lai).
Hiệu ứng:
Hiệu ứng khiên: Sonic phát sáng và có thêm lớp khiên khi sử dụng vật phẩm
Hiệu ứng tăng tốc: Sonic phát sáng khi sử dụng vật phẩm tăng tốc 
Về Source Code Game
Mã nguồn của game được tổ chức thành các file và thư mục:

Thư mục src:
main.cpp: File chính, khởi tạo game, load texture (sonic.png, plane.png), và chạy vòng lặp game.
game.cpp / game.h: Xử lý logic chính của game (di chuyển Sonic, tạo máy bay, tính điểm).
renderer.cpp / renderer.h: Xử lý vẽ các thành phần lên màn hình bằng SDL2.
input.cpp / input.h: Xử lý sự kiện bàn phím và chuột.
utils.cpp / utils.h: Các hàm tiện ích (ví dụ: load texture, kiểm tra va chạm).
entity.cpp / entity.h: Quản lý các thực thể trong game (Sonic, máy bay).
input_handler.cpp / input_handler.h: Xử lý chi tiết các sự kiện đầu vào.
resource_manager.cpp / resource_manager.h: Quản lý tài nguyên (texture, âm thanh).
Thư mục img:
Chứa các file texture: sonic.png, plane.png.
Cấu trúc mã nguồn:
Khởi tạo: Sử dụng SDL2 để tạo window và renderer (main.cpp).
Load tài nguyên: Load texture từ thư mục img (utils.cpp).
Logic game: Xử lý di chuyển, va chạm, và tính điểm (game.cpp, entity.cpp).
Sự kiện: Xử lý đầu vào từ người chơi (input.cpp, input_handler.cpp).
Vẽ: Vẽ các thành phần lên màn hình (renderer.cpp).
