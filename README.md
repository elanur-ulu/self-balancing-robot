# Two-Wheeled Self-Balancing Robot (ROS 2)

Bu proje, iki tekerlekli bir denge robotunun (self-balancing robot) modellenmesi, kinematik yapısının tanımlanması ve **ROS 2** ortamında PID tabanlı kapalı çevrim kontrolünün gerçekleştirilmesini içerir.

---

## Proje Özeti
Robot, gövdesinin dikey eksendeki eğim açısını (pitch) koruyarak dengede kalmak üzere tasarlanmıştır. IMU üzerinden alınan açısal pozisyon ve hız verileri işlenerek **PID kontrol algoritması** ile motorlara gerekli tork/hız komutları iletilir.

---

## Kullanılan Teknolojiler & Araçlar
* **İşletim Sistemi:** Linux (Ubuntu / WSL2)
* **Ara Katman Yazılımı:** ROS 2
* **Modelleme:** URDF (Unified Robot Description Format)
* **Diller:** C++ / Python
* **Algoritma:** PID Kontrol (Proportional-Integral-Derivative)

---

## Paket Yapısı
```text
balance_bot_control/
├── CMakeLists.txt
├── package.xml
├── src/
│   └── balance_node.cpp      # Denge ve PID kontrol düğümü
└── urdf/
    └── balance_bot.urdf      # Robotun mekanik ve görsel URDF modeli
