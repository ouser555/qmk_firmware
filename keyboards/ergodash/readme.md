# Ergodashv99版本說明

    * 目前有三個版本

    * v99v3 焊接版本
        * make ergodash/rev1:v99v3
    
    * v99v4 原版ergodash拉出LED腳位驅動Trackpoint
        * make ergodash/rev1:v99v4
    
    * v99v5 hotswap+RGB版本
        * make ergodash/rev2:v99v5
        
        * keymaps/v99v5/hex/資料夾底下放了left，right，eehand，三種燒錄檔
            * left和right這兩個，一般是兩邊都燒同一個燒錄檔即可，選定左邊或右邊做主控。
            * 也可以左手燒left，右手燒right，這樣就可以兩邊都當主控，也可以個別接一條USB線
                (當個別接USB線時，就當作是兩個鍵盤，像是左邊切層右邊因為沒有連線，所以右邊保持原層)
            * 也可以兩邊都燒eehand的燒錄檔，在燒錄時燒錄eeprom決定是左手或右手，
                * make ergodash/rev2:v99v5:avrdude-split-left
                  (這樣是編譯時同時決定為左手鍵盤，並同時燒錄，訊息到最後需要使用者按下鍵盤的reset鍵，會自動進行燒錄)
                * make ergodash/rev2:v99v5:avrdude-split-right
                  (右手)

# 使用注意事項
    * 如要使用這裡的版本，原版的ergodash/rev1/keymaps 這個資料夾必須刪掉，否則編譯不會過。也可以把整個ergodash整個資料夾刪掉，再複製這裡的版本。

# ErgoDash

![ErgoDash](https://github.com/omkbd/picture/blob/master/Ergodash.jpg)

Keyboard Maintainer: [omkbd](https://github.com/omkbd) [@omkbd](https://twitter.com/omkbd)  
Hardware Supported: ErgoDash PCB, Pro Micro ATmega32u4  
Hardware Availability: Order your own [yourself](https://github.com/omkbd/ErgoDash)


Make example for this keyboard (after setting up your build environment):

    make ergodash/rev1:default

See [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) then the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information.

Install Example: (for pro micro)
    `make ergodash/rev1:default:avrdude`

Note:  
  "rev1" is for PCB ver 1.0,1.1,1.2  
  **The original Rev 1 was owned only by the designer. Therefore, Rev1 has been removed. Since the current PCB is Rev1, we changed Rev2 to Rev1 to match the firmware version.**


# Layout
![layout](https://github.com/omkbd/picture/blob/master/ergodash-layout.png)

Layout Note:  
- In thumb cluster, it is not possible to use all 5 positions as small keys. (as pictured)
- The top 2 1u keys in the thumb cluster share the same connection.
- 2u key (center) position shares with the lower 1u key position

![PCB](https://github.com/omkbd/picture/blob/master/Ergodash_PCB.jpg)
