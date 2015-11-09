using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MathRegression
{
    class MathRegression
    {
        static void Main(string[] args)
        {
            Vec2Regression v2test = new Vec2Regression();
            Vec3Regression v3test = new Vec3Regression();
            Vec4Regression v4test = new Vec4Regression();
            Mtx2Regression m2test = new Mtx2Regression();
            Mtx3Regression m3test = new Mtx3Regression();
            Mtx4Regression m4test = new Mtx4Regression();
            QuatRegression quattest = new QuatRegression();

            v2test.RunTests();
            Console.WriteLine("--");
            v3test.RunTests();
            Console.WriteLine("--");
            v4test.RunTests();
            Console.WriteLine("--");
            m2test.RunTests();
            Console.WriteLine("--");
            m3test.RunTests();
            Console.WriteLine("--");
            m4test.RunTests();
            Console.WriteLine("--");
            quattest.RunTests();
        }
    }
}
