1.Cách Tải Game<br>


2.Cách Chơi<br>
    Chọn Start Game để bắt đầu<br>
    Sonic sẽ tự động chạy về phía trước, và nhiệm vụ của bạn là điều khiển anh ấy để né tránh chướng ngại vật.<br>
    Chọn Màn Chơi<br>
    Game có 2 màn chơi với bối cảnh bầu trời khác nhau:<br>
    Màn 1: Bầu Trời Xanh <br>
    Màn 2: Bầu Trời Hoàng Hôn <br>

2.Các Thành Phần Trong Game<br>
    Sonic (Nhân Vật Chính): Chú nhím Sonic né tránh chướng ngại vật và thu thập vật phẩm.<br>
    Chướng ngại vật (Obstacles): Các chướng ngại vật xuất hiện ngẫu nhiên, cản đường Sonic.<br>
    Điểm Số (Score): Hiển thị ở góc trên cùng của màn hình, tăng dần theo thời gian .<br>

3.Bố cục game cơ bản:<br>
    Sonic chạy từ trái sang phải trên một bầu trời cuộn vô tận. Chướng ngại vật xuất hiện ngẫu nhiên từ bên phải, di chuyển về phía Sonic.

4.Cách Chơi<br>
    Mục tiêu: Điều khiển Sonic để né tránh máy bay địch, thu thập điểm số và chạy càng xa càng tốt.<br>
    Điều khiển:<br>
    Di chuyển: Sử dụng phím space để nhảy.
<br>
  *Tăng tốc: Thu thập vật phẩm tăng tốc để bay nhanh hơn trong thời gian ngắn.<br>
    Khiên bảo vệ: Bảo vệ Sonic khỏi một lần va chạm.<br>
    Chướng Ngại Vật và Kẻ Thù<br>
    Game có 2 loại chướng ngại vật<br>

  *Tên Chướng Ngại Vật	Chức Năng<br>
    Chướng ngại vật bay:<br>
    Chướng ngại vật di chuyển lên xuống:<br>
    Chướng ngại vật hồ nước:<br>
    Chướng ngại vật cao:<br>
    Chiến thắng: Không có điểm kết thúc (endless runner). Mục tiêu là đạt điểm số cao nhất có thể.<br>
    Thất bại: Nếu Sonic va chạm với chướng ngại vật, game sẽ kết thúc. Điểm số của bạn sẽ được hiển thị, và bạn có thể chơi lại.<br>

5.Về Đồ Họa Của Game<br>
    Đồ họa của game được thiết kế đơn giản nhưng bắt mắt:<br>
    Sonic: Sử dụng texture sonic.png, với hoạt ảnh di chuyển mượt mà.<br>
    Nền: Một bầu trời cuộn vô tận với các đám mây (có thể được thêm trong tương lai).<br>
    Hiệu ứng:
    Hiệu ứng khiên: Sonic phát sáng và có thêm lớp khiên khi sử dụng vật phẩm<br>
    Hiệu ứng tăng tốc: Sonic phát sáng khi sử dụng vật phẩm tăng tốc <br>
    Về Source Code Game<br>
    Mã nguồn của game được tổ chức thành các file và thư mục:<br>

Thư mục src:<br>
main.cpp: File chính, khởi tạo game, load texture (sonic.png, plane.png), và chạy vòng lặp game.<br>
game.cpp / game.h: Xử lý logic chính của game (di chuyển Sonic, tạo máy bay, tính điểm).<br>
renderer.cpp / renderer.h: Xử lý vẽ các thành phần lên màn hình bằng SDL2.<br>
input.cpp / input.h: Xử lý sự kiện bàn phím và chuột.<br>
utils.cpp / utils.h: Các hàm tiện ích (ví dụ: load texture, kiểm tra va chạm).<br>
entity.cpp / entity.h: Quản lý các thực thể trong game (Sonic, máy bay).<br>
input_handler.cpp / input_handler.h: Xử lý chi tiết các sự kiện đầu vào.<br>
resource_manager.cpp / resource_manager.h: Quản lý tài nguyên (texture, âm thanh).<br>
Thư mục img:<br>
Chứa các file texture: sonic.png, plane.png.<br>
Cấu trúc mã nguồn:<br>
Khởi tạo: Sử dụng SDL2 để tạo window và renderer (main.cpp).<br>
Load tài nguyên: Load texture từ thư mục img (utils.cpp).<br>
Logic game: Xử lý di chuyển, va chạm, và tính điểm (game.cpp, entity.cpp).<br>
Sự kiện: Xử lý đầu vào từ người chơi (input.cpp, input_handler.cpp).<br>
Vẽ: Vẽ các thành phần lên màn hình (renderer.cpp).<br>
