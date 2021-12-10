## 原本就有Ergodash的修改方式
    * 原始的Ergodash使用的主板是Arduino Pro Micro，檢查過他的線路之後，可以知道他把所有的I/O腳都用上了。
        其實Pro Micro還是有腳位可以生出來的，這邊提供比較容易的三支腳位。
        1.RX左邊第二支腳 PD2
        2.3.最下方的LED上的電阻連接著，PB0和PD5，這應該只有燒錄時可以看到它在閃爍，除此之外沒有被設定其他功能。(如下圖)

![pro_micro_pinout](https://user-images.githubusercontent.com/95702400/145521171-3f43e472-40c0-441b-bc69-28e8c52dca2d.jpg)

        這樣一來就有了三支I/O腳可以使用

    * 這邊要使用的trackpoint模組是V99mini，使用SPI介面
        腳位是1.VCC 2.GND 3.SCLK 4.MOSI 5.MISO 6.CS 7.MOTION
        只要使用三支IO腳就可以驅動了，CS和MOTION腳只要接地(GND)就好了
        V99  -> Pro Micro
        SCLK -> PD2
        MOSI -> PB0
        MISO -> PD5
    
        VCC  -> VCC
        GND  -> GND
        CS   -> GND
        MOT  -> GND
