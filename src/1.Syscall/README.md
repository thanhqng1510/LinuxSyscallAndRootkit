# Syscall


### Yêu cầu:

Cài đặt hai syscall : 
- `int pnametoid(char* name)`: syscall này sẽ nhận vào name của process và trả về pid nếu tìm thấy, trả về -1 nếu không tìm thấy.
- `int pidtoname(int pid, char* buf, int len)`: syscall này sẽ nhận vào pid, ghi process name vào biến buff với max len là len – 1 phần tử, cuối cùng sẽ tự động thêm NULL. Giá trị trả về là -1 nếu lỗi, 0 nếu len buffer truyền vào lớn hơn len của process name, và n với n là độ dài thật sự của process name, trong trường hợp len buffer truyền vào nhỏ hơn len của proccess name.

### Môi trường sử dụng:

- [Ubuntu 12.04.5 32bit LTS.](https://releases.ubuntu.com/12.04/ubuntu-12.04.5-desktop-i386.iso)
- Kernel phiên bản 3.13.0-117-generic.
- Ngôn ngữ C với các thư viện hỗ trợ của Linux.
- Các gói cần cài đặt thêm:
    - `sudo apt-get install libncurses5-dev libncursesw5-dev`.
    - `sudo apt-get install libssl-dev`.
    - `sudo apt-get install libelf-dev`.
    - `sudo apt-get install bison`.
    - `sudo apt-get install flex sudo apt-get install bc`. 
    - `sudo apt-get install perl`.

### Hướng dẫn cài đặt và test syscall:

***Từ đây sẽ mặc định terminal đang ở thư mục 1.Syscall***

**1. Download kernel:**
- Download kernel phiên bản 3.13.0 bằng lệnh `wget  https://mirrors.edge.kernel.org/pub/linux/kernel/v3.0/linux-3.13.tar.xz`.
- Giải nén kernel vào thư mục ***/usr/src/*** bằng lệnh `sudo tar –xvf linux-3.13.tar.xz –C /usr/src/`.

**2. Tạo 2 syscall:**
- Copy 2 folder ***pidtoname*** và ***pnametoid*** vào ***/usr/src/linux-3.13/*** .

**3. Chỉnh sửa Makefile của kernel:**
- Đi đến kernel bằng lệnh `cd /usr/src/linux-13.3/`.
- Mở Makefile của kernel bằng lệnh `sudo gedit Makefile`.
- Sử dụng cộng cụ tìm kiếm đến dòng 
    - `core-y += kernel/ mm/ fs/ ipc/ security/ crypto/ block/`.
- Bổ sung vào cuối dòng tên của 2 folder chứa syscall
    - `core-y += kernel/ mm/ fs/ ipc/ security/ crypto/ block/ pnametoid/ pidtoname/`.
    
**4. Định nghĩa mã cho syscall vừa thêm:**
- Mở file lưu mã của các syscall bằng lệnh `sudo gedit arch/x86/syscalls/syscall_32.tbl`.
- Thêm 2 dòng vào cuối file :
    - `351    i386    pnametoid       sys_process_name`.
    - `352    i386    pidtoname       sys_process_id`.
    
**5. Định nghĩa nguyên hàm trong system call header file:**
- Mở file lưu nguyên hàm của các syscall bằng lệnh `sudo gedit include/linux/syscalls.h`. 
- Thêm 2 dòng sau vào trước dòng ***#endif*** cuối cùng:
    - `asmlinkage long sys_process_name(char* process_name);`.
    - `asmlinkage long sys_process_id(int pid, char* buf, int len);`.
    
**6. Biên dịch và cài đặt kernel:**
- Tại file lưu các config mặc định bằng lệnh `sudo make menuconfig`, chọn ***save*** và ***exit***. 
- Biên dịch kernal bằng lệnh `sudo make -j 2`.
- Cài đặt kernel bằng lệnh `make modules_install install`.
- Khởi động lại máy bằng lệnh `sudo reboot`. Khi khởi động vào ***Ubuntu advance option***, chọn kernel ***linux3.13*** để sử dụng.

**7. Test systemcall:**
- Đi đến thư mục ***1.Syscall***.
- Biên dịch file test bằng lệnh `gcc test.c -o test`.
- Chạy file test bằng lệnh `./test`.




    


    
    


