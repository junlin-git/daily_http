
当SBAR SOURCE为HDMI IN时,SBAR接收到的数据为音频+视频，EP接收到 HDMI TMDS 的 data，并将音频数据解析出来，通过 IIS/SPDIF 传给 SPA300 解码，视频数据则再打包经 HDMI Tx 传送给TV或其他显示器设备；



当SBAR SOURCE为HDMI ARC/eARC时,SBAR接收到的数据只是音频，TV或其他播放源设备，可通过 ARC/ eARC 将音频数据传给 EP92A7E，再由其通过 I2S/SPDIF 传给 SPA300 解码