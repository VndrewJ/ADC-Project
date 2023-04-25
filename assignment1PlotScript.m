%get tables
ps128 = readmatrix('ps128.txt');
ps64 = readmatrix('ps64.txt');
ps32 = readmatrix('ps32.txt');
ps16 = readmatrix('ps16.txt');
ps8 = readmatrix('ps8.txt');
ps4 = readmatrix('ps4.txt');
ps2 = readmatrix('ps2.txt');

%create voltage array
v = linspace(0,253,988);
v = v*5/255;

lim = 988;

%recreate gain error plot from data sheet figure 23-11
figure(1);
y128 = ps128(1:lim, 6)*5/255;
y64 = ps64(1:lim, 6)*5/255;
y32 = ps32(1:lim, 6)*5/255;
y16 = ps16(1:lim, 6)*5/255;
y8 = ps8(1:lim, 6)*5/255;
y4 = ps4(1:lim, 6)*5/255;
y2 = ps2(1:lim, 6)*5/255;

plot(v,y128,'r-',v,y64,'g-',v,y32,'b-',v,y16,'c-',v,y8,'m-',v,y4,'y-',v,y2,'k-',v,v, 'r--');
legend({'128','64','32','16','8','4','2','Ideal'},'Location','northwest');
xlim([0 253]*5/255);
ylim([0 253]*5/255);
xlabel("Ideal ADC signal (V)");
ylabel("Actual ADC signal (V)");
title("Visualisation of Gain Error");

%calc gain error
ge128 = (((ps128(lim, 6))-253)/255)*5;
ge64 = (((ps64(lim, 6))-253)/255)*5;
ge32 = (((ps32(lim, 6))-253)/255)*5;
ge16 = (((ps16(lim, 6))-253)/255)*5;
ge8 = (((ps8(lim, 6))-253)/255)*5;
ge4 = (((ps4(lim, 6))-253)/255)*5;
ge2 = (((ps2(lim, 6))-253)/255)*5;

%plot gain error
figure(2);
error = [ge2,ge4,ge8,ge16,ge32,ge64,ge128];
prescale=[2,4,8,16,32,64,128];
semilogx(prescale,error);
xlim([2 128]);
xticks([2 4 8 16 32 64 128]);
xlabel("ADC Prescale Value");
ylabel("Gain Error (V)");
title("Relationship Between Prescale Value and Gain Error");