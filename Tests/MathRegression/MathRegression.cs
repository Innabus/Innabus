using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Innabus.Math;
using DXVec3 = Microsoft.DirectX.Vector3;

namespace MathRegression
{
    class MathRegression
    {
        static string FormatDXV3(DXVec3 v) { return String.Format("[{0}, {1}, {2}]", v.X, v.Y, v.Z); }

        static void Main(string[] args)
        {
            Vec2Regression v2test = new Vec2Regression();
            Vec3Regression v3test = new Vec3Regression();
            v2test.RunTests();
            Console.WriteLine("--");
            v3test.RunTests();
        }
    }
}
