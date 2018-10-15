using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

//Aforge
using AForge;
using AForge.Controls;
using AForge.Video;
using AForge.Video.DirectShow;
using Size = System.Drawing.Size;

namespace FacialAttractivenessPrediction
{
    public partial class Form1 : Form
    {
        public class FaceData
        {
            public Bitmap bitmap;
            public Rectangle rect;
            public int faceTopLeftX;
            public int faceTopLeftY;
            public int faceWidth;
            public int faceHeight;
        }
        private FaceData faceData = new FaceData();
        private int score = 0;
        private int faceScore = 0;
        private int faceTypeScore = 0;
        private int faceColorScore = 0;
        private FilterInfoCollection videoDevices;
        private VideoCaptureDevice videoDevice;
        private IVideoSource videoSource;
        private Bitmap bitmap = null;   //当前帧
        private BitmapData bmpData;
        private bool isRun = true;

        private int selectedDeviceIndex = 0;

        private Thread thread = new Thread(new ParameterizedThreadStart(drawRect));
        private int fps = 0;
        private int facefps = 0;

        public Form1()
        {
            InitializeComponent();
        }

        public static void drawRect(object obj)
        {
            FaceData faceData = obj as FaceData;
            BitmapData bmpData = faceData.bitmap.LockBits(faceData.rect, ImageLockMode.ReadWrite, faceData.bitmap.PixelFormat);
            Program.detectFace(bmpData.Scan0, bmpData.Width, bmpData.Height,
                ref faceData.faceTopLeftX, ref faceData.faceTopLeftY, 
                ref faceData.faceWidth, ref faceData.faceHeight);
            faceData.bitmap.UnlockBits(bmpData);

        }

        private unsafe void button1_Click(object sender, EventArgs e)
        {
            //Program.faceData ff = new Program.faceData();
            //Program.getFaceDate(ref ff);
            //label1.Text = ff.image[0] + "," + ff.score.ToString();
            //GC.Collect();//手动回收垃圾
            //枚举所有摄像机
            if (isRun)
            {
                MessageBox.Show("评分之前，请先按下拍照按钮", "请先拍照", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            Thread.Sleep(10);
            score = 0;
            bmpData = bitmap.LockBits(faceData.rect, ImageLockMode.ReadWrite, bitmap.PixelFormat);
            if (Program.getScore(bmpData.Scan0, bmpData.Width, bmpData.Height, ref score, ref faceScore, ref faceTypeScore, ref faceColorScore))
            {
                bitmap.UnlockBits(bmpData);
                string outputStr = "比例基础分: " + faceScore.ToString() + "\n肤色分: " +
                    (faceColorScore * 2 / 10).ToString() + "\n脸型分: " + (faceTypeScore * 2 / 10).ToString() + "\n脸型: ";
                switch(faceTypeScore)
                {
                    case 100: outputStr += "倒三角脸"; break;
                    case 90: outputStr += "椭圆形脸"; break;
                    case 80: outputStr += "长方形脸"; break;
                    case 70: outputStr += "圆脸"; break;
                    case 60: outputStr += "国字脸"; break;
                }
                outputStr += "\n得分: " + score.ToString();
                MessageBox.Show(outputStr);
                label1.Text = "得分：" + score.ToString();
            }
            else
            {
                bitmap.UnlockBits(bmpData);
                MessageBox.Show("为了更好地评分，请重新拍一张！", "请重新拍照", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
            isRun = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            isRun = !isRun;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            Text = "颜值评估系统 FPS: " + fps.ToString() + " 识别FPS: " + facefps.ToString();
            fps = 0;
            facefps = 0;
        }

        private unsafe void show(object sender, NewFrameEventArgs eventArgs)
        {
            GC.Collect();
            if (isRun)
            {
                if (faceData.rect.Width == 0 && faceData.rect.Height == 0)
                {
                    faceData.rect = new Rectangle(0, 0, eventArgs.Frame.Width, eventArgs.Frame.Height);
                }
                
                bitmap = eventArgs.Frame.Clone(faceData.rect, eventArgs.Frame.PixelFormat);

                ThreadState state = thread.ThreadState;
                if (state == ThreadState.Stopped || state == ThreadState.Unstarted)
                {
                    if (state == ThreadState.Stopped)
                    {
                        thread = new Thread(new ParameterizedThreadStart(drawRect));
                    }
                    faceData.bitmap = eventArgs.Frame.Clone(faceData.rect, eventArgs.Frame.PixelFormat); ;
                    thread.Start(faceData);
                    facefps++;
                }

                bmpData = bitmap.LockBits(faceData.rect, ImageLockMode.ReadWrite, bitmap.PixelFormat);
                Program.drawRectangle(bmpData.Scan0, bmpData.Width, bmpData.Height,
                    faceData.faceTopLeftX, faceData.faceTopLeftY,
                    faceData.faceWidth, faceData.faceHeight);
                bitmap.UnlockBits(bmpData);
                pictureBox1.Image = bitmap;
                fps++;

                
            }
            else
            {
                Thread.Sleep(1);
            }
        }



        private void Form1_Load(object sender, EventArgs e)
        {
            videoDevices = new FilterInfoCollection(FilterCategory.VideoInputDevice);
            selectedDeviceIndex = 0;
            videoDevice = new VideoCaptureDevice(videoDevices[selectedDeviceIndex].MonikerString);//连接摄像头。
            videoDevice.VideoResolution = videoDevice.VideoCapabilities[selectedDeviceIndex];
            videoSource = videoDevice;
            videoSource.NewFrame += new NewFrameEventHandler(this.show);
            videoSource.Start();

            //videoSourcePlayer1.VideoSource = videoDevice;
            //videoSourcePlayer1.NewFrame += new VideoSourcePlayer.NewFrameHandler(show);
            //videoSourcePlayer1.Start();

            // set NewFrame event handler
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            //DialogResult result = MessageBox.Show("你确定要关闭吗！", "提示信息", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
            //if (result == DialogResult.OK)
            //{
            //    e.Cancel = false;  //点击OK  
            //}
            //else
            //{
            //    e.Cancel = true;
            //}
            videoDevice.Stop();
        }
    }
}
