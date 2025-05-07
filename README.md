# PacMan
 bài tập lớn
Mô hình game: người chơi điều khiển Pacman bằng cách nhấn giữ các phím mũi tên đi hết map, sao cho ăn hết những đồng xu trắng trên màn hình và tránh chướng ngại vật là những con ma.
Tính năng: khi ma di chuyển lại gần Pacman trong bán kính là 7 ô của bản đồ, chúng sẽ chuyển sang chế độ đuổi theo Pacman với đường đi ngắn nhất sử dụng thuật toán A*
MỞ ĐẦU:
- Khi ấn chạy chương trình xuất hiện menu hướng dẫn người chơi cần ấn và giữ những nút mũi tên để điều khiển Pacman di chuyển liên tục
- Ảnh nền của menu là 1 ảnh bình thường sử dụng blendmode để làm mờ đi
- Dưới cùng là nút bật tắt nhạc
CHẾ ĐỘ CHƠI CHÍNH
- Điều khiển Pacman chạy hết map và ăn hết các đồng xu xuất hiện, khi di chuyển, Pacman cần tránh va chạm với những con ma
- Những con ma sẽ di chuyển random những khi lại gần Pacman trong khoảng cách "chase" tính theo ô map, chúng sẽ bắt đầu đuổi Pacman, nhưng vì tốc độ của Pacman nhanh hơn ma, Pacman có thể dễ dàng thoát ra khỏi bán kính bị đuổi
KẾT THÚC
- Sau khi đã ăn hết chấm, người chơi sẽ đến được màn hình "YOU WIN", lúc đó hãy lựa chọn thoát trò chơi
- Nếu Pacman bị va chạm với ma, game sẽ tính số lần va chạm và in ra màn hình result số lần bị chết
CHECK LIST
Tự làm những phần nào, phần nào lấy từ các kiểu tutorial/code mẫu
- Sử dụng những lệnh trong docs được cung cấp
- Tham khảo cách sửa lỗi và các hàm trên YouTube, Google
Liệt kê các nguồn tham khảo (code, hình ảnh, âm thanh...)
- Âm thanh: nhạc TickTack - Illit
- Hình ảnh: tự vẽ, Google
Dùng các lệnh vẽ hình
Texture
Background
Event bàn phím
Event chuột
Animation (hoạt hình)
Xử lý va chạm
Lưu số lần chết trong khi chơi
Background music on/off
Font
Menu
Pause/Resume
