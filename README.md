# Hệ Thống IoT Tưới Cây Tự Động

## Nội Dung Chính
Hệ thống tưới cây tự động sử dụng công nghệ IoT được thiết kế nhằm hỗ trợ người dùng chăm sóc cây trồng hiệu quả và tiết kiệm thời gian. Hệ thống sử dụng cảm biến độ ẩm đất để đo mức độ ẩm của đất và điều khiển máy bơm tưới nước khi cần. Qua ứng dụng di động hoặc giao diện web, người dùng có thể theo dõi thông tin thời gian thực và điều chỉnh chế độ tưới phù hợp với từng loại cây. Đây là giải pháp đơn giản, tiết kiệm, phù hợp cho các hộ gia đình, người yêu cây cảnh hoặc nông nghiệp nhỏ lẻ.
### Các chức năng chính:
- Tự động tưới cây: Kích hoạt bơm khi độ ẩm đất dưới ngưỡng cài đặt và ngắt bơm khi độ ẩm đủ.
- Theo dõi thời gian thực: Hiển thị trạng thái độ ẩm đất qua ứng dụng

## Sơ đồ khối
### Các thành phần chính:
- Cảm biến độ ẩm đất: Đo độ ẩm trong đất, gửi tín hiệu về bộ vi điều khiển.
- ESP32: Xử lý tín hiệu từ cảm biến và điều khiển máy bơm.
- Máy bơm nước: Được điều khiển bật/tắt bởi ESP32 qua relay.
- Relay module: Trung gian điều khiển máy bơm.
- Nguồn cấp điện: Cung cấp năng lượng cho bộ điều khiển và máy bơm.
  ![image](https://github.com/user-attachments/assets/2b1e82fc-29e2-4a55-b67b-914fd22f5455)

