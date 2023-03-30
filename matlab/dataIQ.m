clc;close all;clear;
%% 读取IQ数据
[gnbRx, gnbTx, ueRx, ueRxF, ueRxRb] = readIQ();

%% 初始化数据
subframeSamp = 61440; %一个子帧的样点数（1ms）两个时隙
slotSamp = subframeSamp / 2; %一个时隙的样点数（0.5ms）
ofdmSize = 2048; %fft点数

% plot(dataI(1, :));
ueRxF.I = fftshift(ueRxF.I, 1);
ueRxF.Q = fftshift(ueRxF.Q, 1);

%% test

%测试my_tool里fft函数是否正确
% pss_t=dataI(1,4562:4562+2047)+1i*dataQ(1,4562:4562+2047);
% pssF = fft(pss_t).';
% pssF = fftshift(pssF);
% pssFtemp = ueRxF.I(:,3)+ueRxF.Q(:,3)*1i;

% 计算PSS峰值
% peak = 0;
% peakInd = 1;
% test_subf = 1;
% for i = 1:61440 - 2047
%     temp = ueRx.I(test_subf, i:i+2047) + 1i * ueRx.Q(test_subf, i:i+2047);
%     corr(i) = abs(sum(Pss .* conj(temp)) / 2^11).^2;
%     if(corr(i) > peak)
%         peak = corr(i);
%         peakInd = i;
%     end
% 
% end
% figure;plot(corr);

%% PLOT
% rxData = tempData(1, :) + 1i * tempData(2,:);
% spectrogram(rxData(61440*9:61440 * 15),ones(2048,1),0,2048,'centered',61440000,'yaxis','MinThreshold',-130);

