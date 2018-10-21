using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;



namespace FacialAttractivenessPrediction
{
    static class Program
    {
        [DllImport(@"FaceScoreEval.dll")]
        public static extern unsafe Boolean detectFace(IntPtr data, int imageWidth, int imageHeight, ref int x, ref int y, ref int w, ref int h);
        [DllImport(@"FaceScoreEval.dll")]
        public static extern unsafe bool getScore(IntPtr data, int imageWidth, int imageHeight, 
            ref int totalScore, ref int faceScore, ref int faceTypeScore, ref int faceColorScore);
        [DllImport(@"FaceScoreEval.dll")]
        public static extern unsafe void drawRectangle(IntPtr data, int imageWidth, int imageHeight, int x, int y, int w, int h);

        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        /// 

        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
