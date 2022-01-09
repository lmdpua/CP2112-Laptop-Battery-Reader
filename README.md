# CP2112-Laptop-Battery-Reader
This simple program shows all the parameters of your battery within the Smart Battery Data Specification Revision 1.1<br />
And also some other parameters. Namely, viewing the voltage on each battery cell. Although they are undocumented in this specification, many manufacturers use them.<br />

![Screenshot](https://user-images.githubusercontent.com/17345244/148661589-7d64b022-5c16-4cd5-8fc0-1e56cbf41381.jpg)

I am using <a title="download" href="https://www.qt.io/product/development-tools" target="_blank">Qt Creator 6.0.1</a> <br />
You can examine my bad code and make your changes. <br />
Or, if you are lazy and everything suits you, download the finished release. After you unzip the zip file to a convenient location, run the exe file.<br />
<p>In order to connect to the battery, you will need a small board on the cp2112 chip. There are a lot of them on the Internet, on ebay, aliexpress, and possibly in a radio parts store near you. They are inexpensive, a few dollars.</p>

![cp2112](https://user-images.githubusercontent.com/17345244/148701376-6269510e-795b-49de-9681-04b7a7ec95bf.png)

However, if you would like to delve deeper into this topic, you may be interested in purchasing an <a title="evaluation kit" href="https://www.silabs.com/interface/usb-bridges/classic/device.cp2112" target="_blank">evaluation kit</a>.

![cp2112ek](https://user-images.githubusercontent.com/17345244/148701959-6c730b6e-8f6e-4a66-92cf-d548e4dfa904.png)

I'll tell you a secret that in some cases you can get it for free. For example, for a programming classroom in your school. I only beg you, do not ask me about the details, I do not know these subtleties<br />
And finally you need to connect GND, SDA and SСL to your battery. Be careful! Never connect these contacts to the positive terminal of the battery.
