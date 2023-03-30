function [gnbRx, gnbTx, ueRx, ueRxF, ueRxRb] = readIQ()
fid = fopen('/home/zk/5G-communication/data/gNB/rxdata.iq','rb');  % gNB rx数据文件
[gnbRxTmp, ~] = fread(fid, [2, inf],'int16');

fid = fopen('/home/zk/5G-communication/data/gNB/txdata.iq','rb');  % gNB tx数据文件
[gnbTxTmp, ~] = fread(fid, [2, inf],'int16');

fid = fopen('/home/zk/5G-communication/data/UE/rxdata.iq','rb');  % UE rx数据文件
[ueRxTmp, ~] = fread(fid, [2, inf],'int16');

% fid = fopen('/home/zk/5G-communication/data/UE/pss.iq','rb');  % PSS文件
% [tempPss, ~] = fread(fid, [2, inf],'int16');
% Pss = tempPss(1, :) + 1i * tempPss(2,:);

fid = fopen('/home/zk/5G-communication/data/UE/rxdataF.iq','rb'); % 频域数据文件
[ueRxFTmp, ~] = fread(fid, [2, inf],'int16');

fid = fopen('/home/zk/5G-communication/data/UE/rxdataRB.bin','rb'); % 频域数据文件
[ueRxRb, ~] = fread(fid, [106, inf],'int8');

subframeSamp = 61440; %一个子帧的样点数（1ms）两个时隙
slotSamp = subframeSamp / 2; %一个时隙的样点数（0.5ms）
ofdmSize = 2048; %fft点数

% gNB RX
subframeNum = floor(length(gnbRxTmp) / subframeSamp);
gnbRx.I = reshape(gnbRxTmp(1, 1:subframeNum * subframeSamp), subframeSamp, subframeNum).';
gnbRx.Q = reshape(gnbRxTmp(2, 1:subframeNum * subframeSamp), subframeSamp, subframeNum).';

% gNB TX
subframeNum = floor(length(gnbTxTmp) / subframeSamp);
gnbTx.I = reshape(gnbTxTmp(1, 1:subframeNum * subframeSamp), subframeSamp, subframeNum).';
gnbTx.Q = reshape(gnbTxTmp(2, 1:subframeNum * subframeSamp), subframeSamp, subframeNum).';

% UE Rx
subframeNum = floor(length(ueRxTmp) / subframeSamp);
ueRx.I = reshape(ueRxTmp(1, 1:subframeNum * subframeSamp), subframeSamp, subframeNum).';
ueRx.Q = reshape(ueRxTmp(2, 1:subframeNum * subframeSamp), subframeSamp, subframeNum).';

% UE RxF
symbNum = floor(length(ueRxFTmp) / ofdmSize);
ueRxF.I = reshape(ueRxFTmp(1, 1:symbNum * ofdmSize), ofdmSize, symbNum);
ueRxF.Q = reshape(ueRxFTmp(2, 1:symbNum * ofdmSize), ofdmSize, symbNum);

% UE RxRB



end