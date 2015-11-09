using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Innabus.Math;

namespace MathRegression
{
    class QuatRegression
    {
        public void RunTests()
        {
            Roll = 45;
            Yaw = 10;
            Pitch = 30;

            TestMagnitude();
            TestNormalize();
            TestMul();
            TestInvert();
            TestRotations();
        }

        private string Passed(bool b)
        {
            string s = "Passed";
            if (!b)
                s = "Failed";
            return s;
        }
        
        public void PrintResults()
        {
        }

        private void TestMagnitude()
        {
        }

        private void TestNormalize()
        {
        }

        private void TestMul()
        {
        }

        private void TestInvert()
        {
        }

        private void TestRotations()
        {
            // Test euler angle conversions
            Quaternion eulerq = new Quaternion(Yaw, Pitch, Roll);
            Matrix4 fromeuq = eulerq.GetMatrix();
            Matrix4 eulmtx = Matrix4.RotateEuler(Yaw, Pitch, Roll);

            Quaternion[] euqs = new Quaternion[6];
            euqs[0] = new Quaternion(Yaw, Pitch, Roll);
            euqs[1] = new Quaternion(Yaw, Roll, Pitch);
            euqs[2] = new Quaternion(Pitch, Yaw, Roll);
            euqs[3] = new Quaternion(Pitch, Roll, Yaw);
            euqs[4] = new Quaternion(Roll, Yaw, Pitch);
            euqs[5] = new Quaternion(Roll, Pitch, Yaw);

            Microsoft.DirectX.Quaternion dxypr = Microsoft.DirectX.Quaternion.RotationYawPitchRoll(Yaw, Pitch, Roll);

            float angle;
            Vector3 axis;
            Matrix4.GetAxisAngle(eulmtx, out axis, out angle);
            Matrix4 mtxaaverify = Matrix4.RotateAxisAngle(axis, angle);
            Quaternion aaquat = new Quaternion(angle, axis);
            Matrix4 fromaa = aaquat.GetMatrix();

            Quaternion quatRoll = new Quaternion(Roll, new Vector3(0, 0, 1));
            Quaternion quatYaw = new Quaternion(Yaw, new Vector3(0, 1, 0));
            Quaternion quatPitch = new Quaternion(Pitch, new Vector3(1, 0, 0));
            Quaternion compound = quatRoll * quatPitch * quatYaw;
            Microsoft.DirectX.Quaternion dxRoll = Microsoft.DirectX.Quaternion.RotationAxis(new Microsoft.DirectX.Vector3(0, 0, 1), Roll);
            Microsoft.DirectX.Quaternion dxYaw = Microsoft.DirectX.Quaternion.RotationAxis(new Microsoft.DirectX.Vector3(0, 1, 0), Yaw);
            Microsoft.DirectX.Quaternion dxPitch = Microsoft.DirectX.Quaternion.RotationAxis(new Microsoft.DirectX.Vector3(1, 0, 0), Pitch);
            Microsoft.DirectX.Quaternion dxcompound = Microsoft.DirectX.Quaternion.Multiply(Microsoft.DirectX.Quaternion.Multiply(dxRoll, dxPitch), dxYaw);
            Matrix4 compmtx = compound.GetMatrix();

            Vector3 pt = new Vector3(1, 1, 1);
            Vector4 pt4 = new Vector4(1, 1, 1, 1);
            Microsoft.DirectX.Vector4 dxpt4 = new Microsoft.DirectX.Vector4(1, 1, 1, 1);
            Microsoft.DirectX.Matrix dxmtx = Microsoft.DirectX.Matrix.RotationQuaternion(dxcompound);
            dxpt4.Transform(dxmtx);
            Vector3 res = aaquat.RotatePt(pt);
            Vector4 resmtx = Vector4.Mul(pt4, fromaa);
        }

        float Yaw, Pitch, Roll;
    }
}
