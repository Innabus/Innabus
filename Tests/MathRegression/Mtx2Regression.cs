using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Innabus.Math;
using DXMtx = Microsoft.DirectX.Matrix;
using DXVec2 = Microsoft.DirectX.Vector2;

namespace MathRegression
{
    class Mtx2Regression
    {
        public void RunTests()
        {
            Matrix2 a, b;
            DXMtx dxa, dxb;

            a = new Matrix2(1, 2, 2, 1);
            b = new Matrix2(3, 1, 4, 2);
            dxa = new DXMtx();
            dxb = new DXMtx();
            dxa.M11 = 1;
            dxa.M12 = 2;
            dxa.M21 = 2;
            dxa.M22 = 1;
            dxa.M33 = 1;
            dxa.M44 = 1;
            dxb.M11 = 3;
            dxb.M12 = 1;
            dxb.M21 = 4;
            dxb.M22 = 2;
            RotationAngle = (float)(30 * Math.PI / 180);

            TestAdd(a, b, dxa, dxb);
            TestSub(a, b, dxa, dxb);
            TestMul(a, b, dxa, dxb);
            TestTranspose(a, dxa);
            TestInvert(a, dxa);
            TestRotation();

            PrintResults(a, b);
        }


        private string Passed(bool b)
        {
            string s = "Passed";
            if (!b)
                s = "Failed";
            return s;
        }

        public void PrintResults(Matrix2 a, Matrix2 b)
        {
            Console.WriteLine("Matrix 2 Tests:");
            Console.WriteLine("A: {0}\nB: {1}", a, b);
            Console.WriteLine("A + B: {0} ({1}) -- {2}", AdditionResults[0], AdditionResults[1], Passed(AdditionResults[0] == AdditionResults[1]));
            Console.WriteLine("A - B: {0} ({1}) -- {2}", SubtractionResults[0], SubtractionResults[1], Passed(SubtractionResults[0] == SubtractionResults[1]));
            Console.WriteLine("A * B: {0} ({1}) -- {2}", MultiplyResults[0], MultiplyResults[1], Passed(MultiplyResults[0] == MultiplyResults[1]));
            Console.WriteLine("A^T: {0} ({1}) -- {2}", TransposeResults[0], TransposeResults[1], Passed(TransposeResults[0] == TransposeResults[1]));
            Console.WriteLine("A^-1: {0} ({1}) -- {2}", InverseResults[0], InverseResults[1], Passed(InverseResults[0] == InverseResults[1]));
            Console.WriteLine("Rotation: {0} ({1}) -- {2}", RotationResults[0], RotationResults[1], Passed(RotationResults[0] == RotationResults[1]));
        }

        private Matrix2 DXMtxToMtx2(DXMtx m)
        {
            return new Matrix2(m.M11, m.M12, m.M21, m.M22);
        }

        private void TestAdd(Matrix2 a, Matrix2 b, DXMtx dxa, DXMtx dxb)
        {
            AdditionResults = new Matrix2[2];
            AdditionResults[0] = a + b;
            AdditionResults[1] = DXMtxToMtx2(DXMtx.Add(dxa, dxb));
        }

        private void TestSub(Matrix2 a, Matrix2 b, DXMtx dxa, DXMtx dxb)
        {
            SubtractionResults = new Matrix2[2];
            SubtractionResults[0] = a - b;
            SubtractionResults[1] = DXMtxToMtx2(DXMtx.Subtract(dxa, dxb));
        }

        private void TestMul(Matrix2 a, Matrix2 b, DXMtx dxa, DXMtx dxb)
        {
            MultiplyResults = new Matrix2[2];
            MultiplyResults[0] = a * b;
            MultiplyResults[1] = DXMtxToMtx2(DXMtx.Multiply(dxa, dxb));
        }

        private void TestTranspose(Matrix2 a, DXMtx dxa)
        {
            TransposeResults = new Matrix2[2];
            TransposeResults[0] = Matrix2.Transpose(a);
            TransposeResults[1] = DXMtxToMtx2(DXMtx.TransposeMatrix(dxa));
        }

        private void TestInvert(Matrix2 a, DXMtx dxa)
        {
            InverseResults = new Matrix2[2];
            InverseResults[0] = Matrix2.Invert(a);
            InverseResults[1] = DXMtxToMtx2(DXMtx.Invert(dxa));
        }

        private void TestRotation()
        {
            RotationResults = new Matrix2[2];
            RotationResults[0] = Matrix2.Rotation(RotationAngle);
            RotationResults[1] = DXMtxToMtx2(DXMtx.AffineTransformation2D(1, new DXVec2(0, 0), RotationAngle, new DXVec2(0, 0)));
        }

        float RotationAngle;
        Matrix2[] AdditionResults;
        Matrix2[] SubtractionResults;
        Matrix2[] MultiplyResults;
        Matrix2[] TransposeResults;
        Matrix2[] InverseResults;
        Matrix2[] RotationResults;
    }
}
