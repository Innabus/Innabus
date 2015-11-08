using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Innabus.Math;
using DXVec3 = Microsoft.DirectX.Vector3;

namespace MathRegression
{
    class Vec3Regression
    {
        public void RunTests()
        {
            Vector3 v1 = new Vector3(3, 2, 1);
            Vector3 v2 = new Vector3(-4, 1, 2 );
            DXVec3 dx1 = new DXVec3(3, 2, 1);
            DXVec3 dx2 = new DXVec3(-4, 1, 2 );

            TestMagnitude(v1, v2, dx1, dx2);
            TestNormalize(v1, v2, dx1, dx2);
            TestDot(v1, v2, dx1, dx2);
            TestAdd(v1, v2, dx1, dx2);
            TestSub(v1, v2, dx1, dx2);
            TestScale(v1, v2, dx1, dx2);
            TestTransform(v1, v2, dx1, dx2);

            PrintResults(v1, v2);
        }

        private string Passed(bool b)
        {
            string s = "Passed";
            if (!b)
                s = "Failed";
            return s;
        }

        public void PrintResults(Vector3 v1, Vector3 v2)
        {
            Console.WriteLine("Vector 3 regression tests:");
            Console.WriteLine("A: {0}\nB: {1}", v1, v2);
            Console.WriteLine("Magnitude A: {0} -- {1}", Magnitudes[0], Passed(Magnitudes[0] == Magnitudes[2]));
            Console.WriteLine("Magnitude B: {0} -- {1}", Magnitudes[1], Passed(Magnitudes[1] == Magnitudes[3]));
            Console.WriteLine("MagnitudeSq A: {0} -- {1}", MagnitudesSq[0], Passed(MagnitudesSq[0] == MagnitudesSq[2]));
            Console.WriteLine("MagnitudeSq B: {0} -- {1}", MagnitudesSq[1], Passed(MagnitudesSq[1] == MagnitudesSq[3]));
            Console.WriteLine("Norm A: {0} ({1}) -- {2}", NormResults[0], NormResults[2], Passed(NormResults[0] == NormResults[2]));
            Console.WriteLine("Norm B: {0} ({1}) -- {2}", NormResults[1], NormResults[3], Passed(NormResults[1] == NormResults[3]));
            Console.WriteLine("A Scaled: {0} ({1}) -- {2}", ScaleResults[0], ScaleResults[2], Passed(ScaleResults[0] == ScaleResults[2]));
            Console.WriteLine("B Scaled: {0} ({1}) -- {2}", ScaleResults[1], ScaleResults[3], Passed(ScaleResults[1] == ScaleResults[3]));
            Console.WriteLine("A + B: {0} ({1}) -- {2}", AdditionResults[0], AdditionResults[1], Passed(AdditionResults[0] == AdditionResults[1]));
            Console.WriteLine("A - B: {0} ({1}) -- {2}", SubtractionResults[0], SubtractionResults[1], Passed(SubtractionResults[0] == SubtractionResults[1]));
            Console.WriteLine("A * B: {0} ({1}) -- {2}", DotProducts[0], DotProducts[1], Passed(DotProducts[0] == DotProducts[1]));
            Console.WriteLine("A * M: {0} ({1}) -- {2}", TransformResults[0], TransformResults[2], Passed(TransformResults[0] == TransformResults[2]));
            Console.WriteLine("B * M: {0} ({1}) -- {2}", TransformResults[1], TransformResults[3], Passed(TransformResults[1] == TransformResults[3]));
        }

        private Vector3 DXToV3(DXVec3 dxv)
        {
            return new Vector3(dxv.X, dxv.Y, dxv.Z);
        }

        private void TestMagnitude(Vector3 v1, Vector3 v2, DXVec3 dx1, DXVec3 dx2)
        {
            Magnitudes = new float[4];
            Magnitudes[0] = v1.Magnitude();
            Magnitudes[1] = v2.Magnitude();
            Magnitudes[2] = dx1.Length();
            Magnitudes[3] = dx2.Length();

            MagnitudesSq = new float[4];
            MagnitudesSq[0] = v1.MagnitudeS();
            MagnitudesSq[1] = v2.MagnitudeS();
            MagnitudesSq[2] = dx1.LengthSq();
            MagnitudesSq[3] = dx2.LengthSq();
        }

        private void TestNormalize(Vector3 v1, Vector3 v2, DXVec3 dx1, DXVec3 dx2)
        {
            NormResults = new Vector3[4];
            NormResults[0] = (new Vector3(v1)).Normalize();
            NormResults[1] = (new Vector3(v2)).Normalize();
            NormResults[2] = DXToV3(DXVec3.Normalize(dx1));
            NormResults[3] = DXToV3(DXVec3.Normalize(dx2));
        }

        private void TestDot(Vector3 v1, Vector3 v2, DXVec3 dx1, DXVec3 dx2)
        {
            DotProducts = new float[2];
            DotProducts[0] = Vector3.Dot(v1, v2);
            DotProducts[1] = DXVec3.Dot(dx1, dx2);
        }

        private void TestAdd(Vector3 v1, Vector3 v2, DXVec3 dx1, DXVec3 dx2)
        {
            AdditionResults = new Vector3[2];
            AdditionResults[0] = v1 + v2;
            AdditionResults[1] = DXToV3(dx1 + dx2);
        }

        private void TestSub(Vector3 v1, Vector3 v2, DXVec3 dx1, DXVec3 dx2)
        {
            SubtractionResults = new Vector3[2];
            SubtractionResults[0] = v1 - v2;
            SubtractionResults[1] = DXToV3(dx1 - dx2);
        }

        private void TestScale(Vector3 v1, Vector3 v2, DXVec3 dx1, DXVec3 dx2)
        {
            ScaleResults = new Vector3[4];
            ScaleResults[0] = v1 * 2.0f;
            ScaleResults[1] = v2 * .5f;
            ScaleResults[2] = DXToV3(DXVec3.Multiply(dx1, 2.0f));
            ScaleResults[3] = DXToV3(DXVec3.Multiply(dx2, 0.5f));
        }

        private void TestTransform(Vector3 v1, Vector3 v2, DXVec3 dx1, DXVec3 dx2)
        {
            TransformResults = new Vector3[4];
            float rads = (float)(30 * Math.PI / 180);
            Vector3 axis = (new Vector3(1, 2, 3)).Normalize();

            Matrix3 rotation = Matrix3.RotateAxisAngle(new Vector3(1, 2, 3), rads);
            Microsoft.DirectX.Matrix dxaxisangle = Microsoft.DirectX.Matrix.RotationAxis(new DXVec3(1, 2, 3), rads);

            DXTransformMatrix = Microsoft.DirectX.Matrix.RotationAxis(new DXVec3(1, 2, 3), rads);

            DXTransformResults = new Microsoft.DirectX.Vector4[2];
            DXTransformResults[0] = DXVec3.Transform(dx1, DXTransformMatrix);
            DXTransformResults[1] = DXVec3.Transform(dx2, DXTransformMatrix);

            TransformResults[0] = v1 * rotation;
            TransformResults[1] = v2 * rotation;
            TransformResults[2] = new Vector3(DXTransformResults[0].X, DXTransformResults[0].Y, DXTransformResults[0].Z);
            TransformResults[3] = new Vector3(DXTransformResults[1].X, DXTransformResults[1].Y, DXTransformResults[1].Z);
        }

        float[] Magnitudes;
        float[] MagnitudesSq;
        float[] DotProducts;
        Vector3[] NormResults;
        Vector3[] AdditionResults;
        Vector3[] SubtractionResults;
        Vector3[] ScaleResults;
        Vector3[] TransformResults;
        Microsoft.DirectX.Matrix DXTransformMatrix;
        Microsoft.DirectX.Vector4[] DXTransformResults;
    }
}
