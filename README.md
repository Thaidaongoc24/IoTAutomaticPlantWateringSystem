# Hệ Thống IoT Tưới Cây Tự Động

## Nội Dung Chính
Hệ thống tưới cây tự động sử dụng công nghệ IoT được thiết kế nhằm hỗ trợ người dùng chăm sóc cây trồng hiệu quả và tiết kiệm thời gian. Hệ thống sử dụng cảm biến độ ẩm đất để đo mức độ ẩm của đất và điều khiển máy bơm tưới nước khi cần. Qua ứng dụng di động hoặc giao diện web, người dùng có thể theo dõi thông tin thời gian thực và điều chỉnh chế độ tưới phù hợp với từng loại cây. Đây là giải pháp đơn giản, tiết kiệm, phù hợp cho các hộ gia đình, người yêu cây cảnh hoặc nông nghiệp nhỏ lẻ.
### Các chức năng chính:
- Tự động tưới cây: Kích hoạt bơm khi độ ẩm đất dưới ngưỡng cài đặt và ngắt bơm khi độ ẩm đủ.
- Điều chỉnh ngưỡng độ ẩm đất để bật máy bơm tự động.
- Theo dõi thời gian thực: Hiển thị trạng thái độ ẩm đất qua ứng dụng

## Sơ đồ khối 
![Hình 1](https://github.com/user-attachments/assets/8bc04043-70f5-4151-b568-1f0bcccf6261)
### Các thành phần chính:
1. **Cảm biến độ ẩm đất**:  
   - Đo độ ẩm của đất và gửi tín hiệu về ESP32.  
   - **Hình ảnh minh họa**:  

     <p align="center">
       <img src="https://github.com/user-attachments/assets/7957e755-b9f5-4fa7-b7fd-5d4b8599a986" alt="Cảm biến điện dung" width="45%" />
       <img src="https://github.com/user-attachments/assets/0777d835-77bd-4daf-8dac-1d8a5450e7cc" alt="Cảm biến điện trở" width="45%" />
     </p>  

     *Từ trái qua phải: Cảm biến độ ẩm đất điện dung và cảm biến độ ẩm đất điện trở.* 
   - **Lý do lựa chọn cảm biến điện dung**:  
     - **Độ bền cao**: Không bị oxy hóa hoặc ăn mòn, phù hợp với đất ẩm.  
     - **Chính xác hơn**: Ít bị ảnh hưởng bởi muối, pH, hoặc tạp chất trong đất.  
     - **Tiêu thụ ít năng lượng**: Không cần dòng điện lớn để đo.  
     - **Bảo trì thấp**: Không cần làm sạch thường xuyên.  
     - **Hoạt động ổn định lâu dài**: Không bị thay đổi đặc tính theo thời gian.  
     - **Dễ lắp đặt**: Không phụ thuộc vào vị trí đặt như cảm biến điện trở.  

2. **ESP32**:  
   - Xử lý tín hiệu từ cảm biến.  
   - Điều khiển relay để bật/tắt máy bơm.  
   - Kết nối với ứng dụng hoặc giao diện web qua Wi-Fi.  

3. **Máy bơm nước**:  
   - Bơm nước để tưới cây, được điều khiển qua relay.  

4. **Relay module**:  
   - Là trung gian điều khiển máy bơm theo tín hiệu từ ESP32.  

5. **Nguồn cấp điện**:  
   - Cung cấp năng lượng cho ESP32, cảm biến và máy bơm.  
## Cấu hình Node-red:
### Node-red flow:
![image](https://github.com/user-attachments/assets/5320262d-b8b8-4f6e-9e42-c15133db0577)
### Node-red dashboard:
![image](https://github.com/user-attachments/assets/1320bdc6-7016-4445-a35d-478c810d6fc7)

### Cấu Trúc và Chức Năng

1. **Nhận Dữ Liệu Độ Ẩm Đất**
- **Nút `mqtt in` - "Soil Moisture"**:
  - Chức năng: Nhận dữ liệu về độ ẩm đất từ MQTT.
  - **Topic**: `plant/soil_moisture`
  - **Qos**: 0 (Quality of Service)
  - **Đầu ra**: Dữ liệu độ ẩm đất sẽ được gửi tới các nút tiếp theo để hiển thị và xử lý.
  
2. **Điều Khiển Bơm Nước**
- **Nút `switch` - "Pump Switch"**:
  - Chức năng: Điều khiển bơm nước qua giao diện Dashboard. Người dùng có thể bật/tắt bơm bằng cách sử dụng công tắc.
  - **Label**: "Water NOW" (Tưới ngay)
  - **On/Off values**: 
    - Khi bật: Gửi giá trị `ON` qua MQTT để kích hoạt bơm.
    - Khi tắt: Gửi giá trị `OFF` qua MQTT để tắt bơm.
  - **Topic**: `plant/pump_control` - Chủ đề điều khiển bơm.

- **Nút `mqtt out` - "Pump Control MQTT"**:
  - Chức năng: Gửi tín hiệu điều khiển bơm qua MQTT.
  - **Topic**: `plant/pump_control`
  - **Qos**: 0
  - **Đầu ra**: Tín hiệu ON/OFF được gửi tới chủ đề MQTT để điều khiển bơm.

3. **Cài Đặt Ngưỡng Độ Ẩm**
- **Nút `slider` - "Set Moisture Threshold"**:
  - Chức năng: Cho phép người dùng đặt ngưỡng độ ẩm mà hệ thống sẽ sử dụng để quyết định khi nào bật bơm nước.
  - **Label**: "Soil Moisture Threshold"
  - **Đầu ra**: Giá trị ngưỡng độ ẩm được gửi qua MQTT đến chủ đề `plant/soil_moisture_threshold`.

- **Nút `mqtt out` - "Threshold MQTT"**:
  - Chức năng: Gửi giá trị ngưỡng độ ẩm qua MQTT.
  - **Topic**: `plant/soil_moisture_threshold`
 - **Qos**: 0

4. **Hiển Thị Dữ Liệu**
- **Nút `gauge` - "Soil Moisture Gauge"**:
  - Chức năng: Hiển thị độ ẩm đất hiện tại dưới dạng đồng hồ đo (gauge).
  - **Chỉ số**: Độ ẩm được hiển thị theo phần trăm (0% đến 100%).
  - **Màu sắc**: Độ ẩm sẽ được hiển thị với các màu sắc khác nhau tùy theo mức độ (ví dụ: vàng cho độ ẩm thấp, xanh lá cho độ ẩm cao).

- **Nút `chart` - "Soil Moisture History"**:
  - Chức năng: Hiển thị biểu đồ lịch sử độ ẩm đất.
  - **Loại biểu đồ**: Biểu đồ đường (line chart) thể hiện sự thay đổi của độ ẩm theo thời gian.
  - **Y-axis**: Mỗi điểm trên biểu đồ đại diện cho một giá trị độ ẩm.

- **Nút `text` - "Moisture Threshold"**:
  - Chức năng: Hiển thị giá trị ngưỡng độ ẩm hiện tại trên giao diện Dashboard.
## Hình ảnh thực tế:
![image](https://github.com/user-attachments/assets/b47fc68b-7461-408a-8620-2435598cd719)

## Video demo:
https://github.com/user-attachments/assets/280fb81e-116d-4afb-9965-f922740a47d6

## Hướng Phát Triển Đề Tài
**Tự Động Hóa Tưới Nước**: Áp dụng AI/ML để dự đoán nhu cầu tưới nước dựa trên dữ liệu lịch sử và thời tiết.

**Tích Hợp Cảm Biến**: Thêm cảm biến nhiệt độ, độ ẩm không khí, và ánh sáng để tối ưu hóa việc tưới nước.

**Nâng Cấp Giao Diện Dashboard**: Hiển thị thêm các thông số như pH đất, mức nước, và tùy chỉnh giao diện người dùng.

**Ứng Dụng IoT**: Điều khiển và giám sát từ xa qua ứng dụng di động hoặc web, và gửi thông báo khi cần.

**Tiết Kiệm Năng Lượng**: Tích hợp năng lượng mặt trời và tự động tắt thiết bị không cần thiết để tiết kiệm năng lượng.

**Tối Ưu Hóa Hệ Thống**: Phân tích hiệu suất và cải tiến mô hình tưới nước để sử dụng tài nguyên hiệu quả hơn.

**Bảo Mật**: Thêm tính năng xác thực người dùng và giám sát từ xa để tăng cường an ninh.
