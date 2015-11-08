using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Innabus.Math;
using DXMtx = Microsoft.DirectX.Matrix;

namespace MathRegression
{
    class Mtx4Regression
    {
        public void RunTests()
        {
            Matrix4 a, b;
            DXMtx dxa, dxb;

            a = new Matrix4(3, 1, 2, 1, 1, 1, 2, 1, 2, 3, 1, 2, 2, 1, 3, 2);
            b = new Matrix4(4, 2, 1, 2, 3, 1, 2, 1, 2, 1, 2, 3, 1, 1, 2, 4);
            dxa = new DXMtx();
            dxb = new DXMtx();

            dxa.M11 = 3; dxa.M12 = 1; dxa.M13 = 2; dxa.M14 = 1;
            dxa.M21 = 1; dxa.M22 = 1; dxa.M23 = 2; dxa.M24 = 1;
            dxa.M31 = 2; dxa.M32 = 3; dxa.M33 = 1; dxa.M34 = 2;
            dxa.M41 = 2; dxa.M42 = 1; dxa.M43 = 3; dxa.M44 = 2;
            dxb.M11 = 4; dxb.M12 = 2; dxb.M13 = 1; dxb.M14 = 2;
            dxb.M21 = 3; dxb.M22 = 1; dxb.M23 = 2; dxb.M24 = 1;
            dxb.M31 = 2; dxb.M32 = 1; dxb.M33 = 2; dxb.M34 = 3;
            dxb.M41 = 1; dxb.M42 = 1; dxb.M43 = 2; dxb.M44 = 4;

            Roll = 45;
            Yaw = 10;
            Pitch = 30;

            TestAdd(a, b, dxa, dxb);
            TestSub(a, b, dxa, dxb);
            TestMul(a, b, dxa, dxb);
            TestTranspose(a, dxa);
            TestInvert(a, dxa);
            TestRotations();

            PrintResults(a, b);
        }


        private string Passed(bool b)
        {
            string s = "Passed";
            if (!b)
                s = "Failed";
            return s;
        }

        public void PrintResults(Matrix4 a, Matrix4 b)
        {
            Console.WriteLine("Matrix 4 Tests:");
            Console.WriteLine("A: {0}\nB: {1}", a, b);
            Console.WriteLine("A + B: {0} ({1}) -- {2}", AdditionResults[0], AdditionResults[1], Passed(AdditionResults[0] == AdditionResults[1]));
            Console.WriteLine("A - B: {0} ({1}) -- {2}", SubtractionResults[0], SubtractionResults[1], Passed(SubtractionResults[0] == SubtractionResults[1]));
            Console.WriteLine("A * B: {0} ({1}) -- {2}", MultiplyResults[0], MultiplyResults[1], Passed(MultiplyResults[0] == MultiplyResults[1]));
            Console.WriteLine("A^T: {0} ({1}) -- {2}", TransposeResults[0], TransposeResults[1], Passed(TransposeResults[0] == TransposeResults[1]));
            Console.WriteLine("A^-1: {0} ({1}) -- {2}", InverseResults[0], InverseResults[1], Passed(InverseResults[0] == InverseResults[1]));
            Console.WriteLine("Basic rotation results -- {2}\n{0}\n{1}", BasicRotations[0], BasicRotations[1], Passed(BasicRotations[0] == BasicRotations[1]));
            Console.WriteLine("Euler rotation results -- {2}\n{0}\n{1}", EulerRotations[0], EulerRotations[1], Passed(EulerRotations[0] == EulerRotations[1]));
            Console.WriteLine("AxisAngle rotation results -- {2}\n{0}\n{1}", AxisAngleRotations[0], AxisAngleRotations[1], Passed(AxisAngleRotations[0] == AxisAngleRotations[1]));
            Console.WriteLine("Quaternion rotation results -- {2}\n{0}\n{1}", QuaternionRotations[0], QuaternionRotations[1], Passed(QuaternionRotations[0] == QuaternionRotations[1]));

            Console.WriteLine("Basic == Euler: {0}", Passed(BasicRotations[0] == EulerRotations[0]));
            Console.WriteLine("Basic == AxisAngle {0}", Passed(BasicRotations[0] == AxisAngleRotations[0]));
            Console.WriteLine("Basic == Quaternion {0}", Passed(BasicRotations[0] == QuaternionRotations[0]));
        }

        private Matrix4 DXMtxToMtx4(DXMtx m)
        {
            return new Matrix4(m.M11, m.M12, m.M13, m.M14, m.M21, m.M22, m.M23, m.M24, m.M31, m.M32, m.M33, m.M34, m.M41, m.M42, m.M43, m.M44);
        }

        private void TestAdd(Matrix4 a, Matrix4 b, DXMtx dxa, DXMtx dxb)
        {
            AdditionResults = new Matrix4[2];
            AdditionResults[0] = a + b;
            AdditionResults[1] = DXMtxToMtx4(DXMtx.Add(dxa, dxb));
        }

        private void TestSub(Matrix4 a, Matrix4 b, DXMtx dxa, DXMtx dxb)
        {
            SubtractionResults = new Matrix4[2];
            SubtractionResults[0] = a - b;
            SubtractionResults[1] = DXMtxToMtx4(DXMtx.Subtract(dxa, dxb));
        }

        private void TestMul(Matrix4 a, Matrix4 b, DXMtx dxa, DXMtx dxb)
        {
            MultiplyResults = new Matrix4[2];
            MultiplyResults[0] = a * b;
            MultiplyResults[1] = DXMtxToMtx4(DXMtx.Multiply(dxa, dxb));
        }

        private void TestTranspose(Matrix4 a, DXMtx dxa)
        {
            TransposeResults = new Matrix4[2];
            TransposeResults[0] = Matrix4.Transpose(a);
            TransposeResults[1] = DXMtxToMtx4(DXMtx.TransposeMatrix(dxa));
        }

        private void TestInvert(Matrix4 a, DXMtx dxa)
        {
            InverseResults = new Matrix4[2];
            InverseResults[0] = Matrix4.Invert(a);
            InverseResults[1] = DXMtxToMtx4(DXMtx.Invert(dxa));
        }

        private void TestRotations()
        {
            Matrix4 rotx = Matrix4.RotateX(Pitch);
            Matrix4 roty = Matrix4.RotateY(Yaw);
            Matrix4 rotz = Matrix4.RotateZ(Roll);
            DXMtx dxrotx = DXMtx.RotationX(Pitch);
            DXMtx dxroty = DXMtx.RotationY(Yaw);
            DXMtx dxrotz = DXMtx.RotationZ(Roll);

            BasicRotations = new Matrix4[2];
            BasicRotations[0] = rotz * rotx * roty;
            BasicRotations[1] = DXMtxToMtx4(DXMtx.Multiply(DXMtx.Multiply(dxrotz, dxrotx), dxroty));

            EulerRotations = new Matrix4[2];
            EulerRotations[0] = Matrix4.RotateEuler(Yaw, Pitch, Roll);
            EulerRotations[1] = DXMtxToMtx4(DXMtx.RotationYawPitchRoll(Yaw, Pitch, Roll));

            Matrix4.GetAxisAngle(EulerRotations[0], out RotationAxis, out RotationAngle);
            AxisAngleRotations = new Matrix4[2];
            AxisAngleRotations[0] = Matrix4.RotateAxisAngle(RotationAxis, RotationAngle);
            AxisAngleRotations[1] = DXMtxToMtx4(DXMtx.RotationAxis(new Microsoft.DirectX.Vector3(RotationAxis.x, RotationAxis.y, RotationAxis.z), RotationAngle));

            Quaternion quat = new Quaternion(RotationAngle, RotationAxis);
            Microsoft.DirectX.Quaternion dxquat = new Microsoft.DirectX.Quaternion(quat.x, quat.y, quat.z, quat.w);
            QuaternionRotations = new Matrix4[2];
            QuaternionRotations[0] = Matrix4.RotateQuaternion(quat);
            QuaternionRotations[1] = DXMtxToMtx4(DXMtx.RotationQuaternion(dxquat));
        }

        float Yaw, Pitch, Roll;
        Vector3 RotationAxis;
        float RotationAngle;
        Matrix4[] AdditionResults;
        Matrix4[] SubtractionResults;
        Matrix4[] MultiplyResults;
        Matrix4[] TransposeResults;
        Matrix4[] InverseResults;
        Matrix4[] BasicRotations;
        Matrix4[] EulerRotations;
        Matrix4[] AxisAngleRotations;
        Matrix4[] QuaternionRotations;
    }
}
