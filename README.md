# Chess_Project
A simple project game using SFML Library and C++ Programming

## 1. Hướng dẫn cài đặt
- Tạo một folder mới để chứa các nội dung của game.
- Sử dụng Visual Studio, trong mục Get Started chọn Clone a Repository
- Tại mục Repository location, nhập vào đường link: https://github.com/dangquangUetK66/Chess_Project.git
- Tại mục Path, chọn nơi bạn muốn lưu trữ game, cụ thể ở đây sẽ chọn Path chỉ đến Folder mới vừa được tạo ở trên
- Sau khi đã clone xong, mở file MenuSFML.sln trong mục Solution Explorer
- Trong project khi đó đã có đầy đủ các file .h và .cpp, bấm vào Local Windows Debugger để chạy chương trình.
- Sau khi bấm vào, ta sẽ thấy không chạy được do thiếu các file .dll ở phần debug
- Ta copy các file .dll trong folder mới vào thư mục x64/Debug
- Sau khi copy xong, nhấn chọn Local Windows Debugger để chạy chương trình

## 2. Mô tả chung về trò chơi
- Game dựa trên ý tưởng về trò chơi cờ vua truyền thống, có điều chỉnh để phù hợp với cách chơi cờ vua dân dã
- Game có 2 chế độ: chế độ chơi người với người (Player vs Player) và chế độ chơi người với máy (Player vs Computer)
- Một Menu hiển thị các mục của trò chơi, trong đó có mục hiển thị Rule & About (Luật chơi) với mặc định quân cờ trắng đi trước
- Trò chơi sẽ kết thúc khi quân tướng của một trong hai bên bị ăn mất
- Luật chơi vẫn theo luật cờ vua hiện hành, có một số hạn chế như chưa có chức năng nhập thành, chiếu bí, chiến thuật của AI còn hạn chế.

## 3. Các chức năng đã cài đặt

### Giao diện người dùng:
- Màn hình Menu:
  - Nút Player vs Computer: chế độ chơi người với máy
  - Nút Player vs Player: chế độ chơi người với người
  - Nút Rule & About: hiển thị luật chơi, các phần liên quan đến trò chơi
  - Nút Exit: thoát khỏi trò chơi

- Màn hình khi chơi game: thoát màn hình bằng nút X bên góc phải, khi đó sẽ trở về Menu

### Xử lý nước đi của trò chơi
- Hiển thị cho người chơi những nước đi hợp lệ trong game
- Có chức năng phong hậu cho tốt khi chạm ô dưới cùng
- Có chức năng Undo khi bấm Space
- Game lập tức kết thúc khi tướng của 1 trong 2 bên bị ăn mất

- Link video preview: https://www.youtube.com/watch?v=3ZjT9FJL2EU

## 4. Các kĩ thuật được sử dụng trong game
- Thư viện đồ họa SFML: https://www.sfml-dev.org/tutorials/2.5/
- Kĩ thuật lập trình được sử dụng:
  - Sử dụng các hàm tham chiếu, tham trị, hằng tham chiếu
  - Struct để quản lí quân cờ, class để quản lí chế độ chơi 1 hay 2 player
  - Mảng, duyệt mảng bàn cờ vua
  - Vector để kiểm tra điều kiện kết thúc game
  - Stack để phục vụ việc Undo quân cờ
  - Thuật toán tỉa cây để phục vụ cho máy chơi (CÓ THAM KHẢO TRÊN MẠNG)

## 5. Kết luận, hướng phát triển và điều tâm đắc

### Kết luận:
  - Chương trình với độ phức tạp ở mức đơn giản, không sử dụng các cấu trúc dữ liệu phức tạp. Trong game phải cài đặt nhiều trường hợp để kiểm tra nước đi của quân cờ
  - Về đồ họa: ở mức vừa phải, chưa quá đẹp và có nhiều animation, tuy nhiên người chơi vẫn có thể dễ dàng sử dụng
  - Về code: đã có refactor code, tuy nhiên một số chức năng code vẫn còn khá dài, cần thêm thời gian để phát triển và refactor
  - Về thư viện đồ họa: SFML đòi hỏi chúng ta phải code thật tỉ mỉ, vì có nhiều chi tiết trong game mà chúng ta khó quản lí
### Hướng phát triển:
  - Cải tiến thêm về mặt đồ họa và âm thanh của game
  - Bổ sung các chức năng mới như nhập thành, chiếu bí, cờ chớp có giới hạn thời gian, ...
  - Cải thiện thêm về AI cờ vua (sử dụng Engine bên ngoài)
### Điều tâm đắc sau khi hoàn thành Project
  - Có thêm kĩ năng tách hàm, tách file code để chương trình dễ quản lí và dễ debug
  - Trước khi bắt đầu viết một chương trình cần vạch ra tỉ mỉ các bước, các phần mà bản thân phải quản lí.
  - Biết tham khảo từ bạn bè, tham khảo các source code trên mạng, tự refactor theo hướng bản thân mong muốn.
  - Có thêm kĩ năng sử dụng các cấu trúc liên quan đến Struct và Class, điều mà trong môn học Nhập môn lập trình chưa có được thực hành nhiều, từ đó phục vụ cho các môn học khó hơn như Lập trình hướng đối tượng
  - Biết thêm về luật cờ vua, một trò chơi thú vị mà bản thân không thật sự có niềm yêu thích
