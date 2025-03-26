# 🌆 Fall Gravity
  ### Tác giả: Đào Bá Anh Quân
  ### Mã sinh viên: 24022713
  ### Trường Đại học Công nghệ - Đại học Quốc gia Hà Nội

# 📃 Mô tả
Fall Gravity là một trò chơi platformer 2D đơn giản được viết bằng C++ sử dụng thư viện SDL. Người chơi điều khiển một nhân vật vượt qua các đoạn sàn bị ngắt quãng để đến cổng đích, trong khi tránh rơi xuống dưới màn hình.
  ## Tính năng chính:
  - Điều khiển: A/D (trái/phải), W (nhảy).
  - Vùng hoạt động: 856x121 pixel, căn giữa màn hình 1089x612.
  - Sàn tĩnh với các lỗ, cổng đích, và logo hiển thị.
  - Xử lý va chạm và trọng lực đơn giản.
  ## Công nghệ:
  - Ngôn ngữ: C++.
  - Thư viện: SDL2, SDL_image.

# 🛠 Yêu cầu hệ thống
- Hệ điều hành: Windows, Linux, hoặc macOS (đã kiểm tra trên Windows).
- Công cụ biên dịch: G++ hoặc bất kỳ trình biên dịch C++ nào hỗ trợ C++11 trở lên.
- Thư viện cần thiết:
  + SDL2
  + SDL2_image
- Yêu cầu bổ sung cho Windows:
  + **Cần cài đặt SDL2 và SDL2_image trong Code::Blocks.**
  + **Hai file SDL2.dll và SDL2_image.dll phải được sao chép vào thư mục chứa mã nguồn (nơi file .cbp hoặc file thực thi sau khi build) để chạy trò chơi.**

# Cách chơi
Mục tiêu: Đưa nhân vật đến cổng ở phía bên phải màn hình.
  ## Điều khiển:
  - A hoặc Mũi tên trái: Di chuyển trái.
  - D hoặc Mũi tên phải: Di chuyển phải.
  - W hoặc Mũi tên lên: Nhảy (chỉ nhảy được khi đứng trên sàn).
  - Thua: Nếu nhân vật rơi khỏi màn hình, nhấn phím bất kỳ để chơi lại.
  - Thắng: Chạm vào cổng để qua màn.
