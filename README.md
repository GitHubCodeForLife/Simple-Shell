# Simple Shell - `DCBA`



### Thông tin đồ án

-   Đồ án cho môn Hệ Điều Hành , FIT @ HCMUS
-   Nhóm gồm 3 thành viên
    -   `18120592`: Nguyễn Phú Bảo Tiến
    -   `18120629`: Trần Văn Tú
    -   `18120636`: Trần Ngọc Tuấn

#### Tất cả các chức năng đều đạt độ hoàn thiện 99%

### Hướng thiết kế của đồ án
##### Ngôn ngữ và môi trường lập trình

Đồ án được lập trình bằng ngôn ngữ `C`,biên dịch bằng `gcc` trên hệ điều hành Linux 


##### Kịch bản của chương trình

0. Dùng `makefile` để build chương trình.
1. Khi shell bắt đầu chạy, chương trình sẽ vào vòng lặp `while` thực hiện chương trình.
2. Chương trình sẽ chạy hàm `prompt` in ra cửa số làm việc, người dùng nhập command vào
3. Command sẽ được đưa vào kiểm tra `BuiltIn` (Built In là chương trình những lệnh đặc biệt mà execvp không xử lý được như: history, cd, !!, help, hello)
4. Lưu lại lịch sử command
5. Kiểm tra command có `Pipe` không. Nếu có thì thực hiện Pipe rồi quay lại bước 2 tiếp tục thực hiện command mới.
6. Kiểm tra command có `chuyển hướng` hay không. Nếu có thì thực hiện hàm chuyển hướng rồi quay lại bước 2 tiếp tục thực hiện command mới.
7. Kiểm tra chương trình có `chạy ngầm` hay không. Nếu có chạy ngầm cờ `background` sẽ được bật lên 1, nếu không thì cờ `background` sẽ được giữ lại 0.
8. Thực thi chương trình trong hàm `execvpArgs`. Sau đó quay lại bước 2.

Note: để thoát chương trình chúng ta có thể sử dụng lệnh exit, hoặc nhấm phím tổ hợp `Ctr + C` hoặc `Ctr + Z`.


##### Các hàm quan trọng
1. Hàm `tokenWord` dùng để tách chuỗi input theo khoảng cách, khoảng trống, dấu tab,.. Kết quả hàm này sẽ thu được là một mảng hai chiều các chuỗi đã nhập.
	Ví dụ: chuỗi str= "echo hello"
		sau khi chạy hàm TokenWord sẽ thu được mảng argv như sau: 
		argv[0] = "echo"
		argv[1] = "hello"
2. Hàm `separateString` dùng để tách chuỗi input đầu vào trong trường hợp có Pipe ('|').
	Hàm return 1: khi command có pipe, return 0: khi command không có pipe. 
	Trong trường hợp có pipe hàm sẽ tách chuỗi input thành 2 chuỗi argv và argv_right.
	vd: chuỗi input = "ls -l|less"
		Sau khi chạy hàm separateString sẽ trả về kết quả là 1 và sẽ tách chuỗi input thành hai mảng argv, argv_right.
		
		argv[0] = "ls"
		argv[1] = "-l"
		
		argv_right[0]="less"
3. Hàm `execArgsBuiltIn` dùng để kiểm tra và thực thi các câu lệnh thuộc loại BuiltIn (BuiltIn là những câu lệnh như: history, cd, !!, help, hello ) 
4. Hàm `execArgs` dùng để thực thi những command line đơn giản như: echo hello, ls, cat, ...
5. 5.	Hàm execArgsPiped thực hiện pipe nhận hai lệnh được tách ra từ hàm separateString
Nguyên lí thiết kế của hàm là tạo ra một ống ‘pipe’ bằng hàm hệ thống pipe(), sau đó chương trình cha sẽ tạo ra 2 tiến trình con chạy song song, 2 tiến trình này sẽ gửi nhận dữ liệu thông qua đường ống ‘pipe’ đã tạo. Chương trình cha sẽ dừng lại đợi đến khi 2 tiến trình con hoàn thành công việc.
6. Hàm `execArgsRedirector` thực thi các câu lệnh trong trường hợp có chuyển hướng.


