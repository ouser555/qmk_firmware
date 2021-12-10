## 原本就有Ergodash加上Trackpoint的修改方式
    * 原始的Ergodash使用的主板是Arduino Pro Micro，檢查過他的線路之後，可以知道他把所有的I/O腳都用上了。
        其實Pro Micro還是有腳位可以生出來的，這邊提供比較容易的三支腳位。
        1.RX左邊第二支腳 PD2
        2.3.最下方的LED上的電阻連接著，PB0和PD5，這應該只有燒錄時可以看到它在閃爍，除此之外沒有被設定其他功能。(如下圖)
        這樣一來就有了三支I/O腳可以使用

![pro_micro_pinout](https://user-images.githubusercontent.com/95702400/145521171-3f43e472-40c0-441b-bc69-28e8c52dca2d.jpg)

    * 這邊要使用的trackpoint模組是V99mini，使用SPI介面

![m001](https://user-images.githubusercontent.com/95702400/145525083-018fad14-ac56-4157-9158-77e07f6daf5f.jpg)

        腳位是1.VCC 2.GND 3.MISO 4.MOTION 5.SCLK 6.CS 7.MOSI  
        只要使用三支IO腳就可以驅動了，CS和MOTION腳只要接地(GND)就好了
        V99  ->  Pro Micro
        ---------------------
        SCLK ->  PD2
        MOSI ->  PB0
        MISO ->  PD5
    
        VCC  ->  VCC
        GND  ->  GND
        CS   ->  GND
        MOT  ->  GND
        
        實接圖，燒錄這個v99v4 make出來的燒錄檔即可動作，不用接其他電路
![005](https://user-images.githubusercontent.com/95702400/145525436-7f2cb1ec-6787-40f4-8be3-d498e8532034.jpg)

        正面
![004](https://user-images.githubusercontent.com/95702400/145525451-fbbf9760-2b07-483e-a4f6-a4149ae7221d.jpg)

        反面
![003](https://user-images.githubusercontent.com/95702400/145525458-02c49372-c1b7-46ea-bcdc-a878d347b865.jpg)

        可以正常動作，但是LED會亮著，如果不喜歡LED亮著的話的話，把電阻拿掉即可
![006](https://user-images.githubusercontent.com/95702400/145525479-1b3d7ba5-532f-4b49-a56e-c0bbdbd9c977.jpg)

        操作展示，隨便找一片空板固定住模組
![VID_20211210_133434](https://user-images.githubusercontent.com/95702400/145525501-7602dffd-74da-4e96-95fe-c64486629635.gif)
