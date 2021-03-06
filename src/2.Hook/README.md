# Hook

### Yêu cầu:

Hook vào 2 syscall dưới đây:
- Syscall `open` => ghi vào `dmesg` tên tiến trình mở file và tên file được mở.
- Syscall `write` => ghi vào `dmesg` tên tiến trình, tên file bị ghi và số byte được ghi.

### Môi trường sử dụng:

- [Ubuntu 14.04.6 32bit LTS.](http://releases.ubuntu.com/14.04/ubuntu-14.04.6-desktop-i386.iso)
- Kernel phiên bản 4.4.0-148-generic.
- Ngôn ngữ C với các thư viện hỗ trợ của Linux.
- Cài đặt thêm gói ***build-essential*** bằng lệnh `sudo apt-get install build-essential`.

### Hướng dẫn sử dụng module:

***Từ đây sẽ mặc định terminal đang ở thư mục 2.Hook***

**1. Cách 1: sử dụng lệnh `make`:**
- Chạy lệnh `sudo make` để build module.
- Cài đặt module bằng lệnh `sudo insmod Hook.ko`.
- Chạy lệnh `dmesg` để kiểm tra.
- Gỡ cài đặt module bằng lệnh `sudo rmmod Hook`.

**2. Cách 2: sử dụng lệnh `make auto`:**
- Chạy lệnh `sudo make auto` để thực hiện toàn bộ các bước trên.
- Chạy lệnh `dmesg` để kiểm tra.
