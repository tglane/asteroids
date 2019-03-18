/*
 * Matrix.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "Matrix.hpp"

namespace asteroids
{

Matrix::Matrix()
{
    for(int i = 0; i < 16; i++) m[i] = 0;
    m[0] = m[5] = m[10] = m[15] = 1;
}


Matrix::Matrix(float* matrix)
{
    for(int i = 0; i < 16; i++) m[i] = matrix[i];
}

    
Matrix::Matrix(const Matrix& other)
{
    for(int i = 0; i < 16; i++) m[i] = other[i];
}

Matrix::Matrix(const Vector3f& a, float angle)
{
    Vector3f axis(a);
    // Check for gimbal lock
    if(fabs(angle) < 0.0001)
    {
        
        bool invert_z = axis[2] < 0;
        
        //Angle to yz-plane
        float pitch = atan2(axis[2], axis[0]) - M_PI_2;
        if(pitch < 0.0f) pitch += 2.0f * M_PI;
        
        if(axis[0] == 0.0f && axis[2] == 0.0) pitch = 0.0f;
        
        //Transform axis into yz-plane
        axis[0] =  axis[0] * cos(pitch) + axis[2] * sin(pitch);
        axis[2] = -axis[0] * sin(pitch) + axis[2] * cos(pitch);
        
        //Angle to y-Axis
        float yaw = atan2(axis[1], axis[2]);
        if(yaw < 0) yaw += 2 * M_PI;
        
        Matrix m1, m2, m3;
        
        if(invert_z) yaw = -yaw;
        
        cout << "YAW: " << yaw << " PITCH: " << pitch << endl;
        
        if(fabs(yaw)   > 0.0001){
            m2 = Matrix(Vector3f(1.0, 0.0, 0.0), yaw);
            m3 = m3 * m2;
        }
        
        if(fabs(pitch) > 0.0001){
            m1 = Matrix(Vector3f(0.0, 1.0, 0.0), pitch);
            m3 = m3 * m1;
        }
        
        for(int i = 0; i < 16; i++) m[i] = m3[i];
        
    } else {
        float c = cos(angle);
        float s = sin(angle);
        float t = 1.0f - c;
        float tmp1, tmp2;
        
        // Normalize axis
        Vector3f a(axis);
        a.normalize();
        
        m[ 0] = c + a[0] * a[0] * t;
        m[ 5] = c + a[1] * a[1] * t;
        m[10] = c + a[2] * a[2] * t;
        
        tmp1 = a[0] * a[1] * t;
        tmp2 = a[2] * s;
        m[ 4] = tmp1 + tmp2;
        m[ 1] = tmp1 - tmp2;
        
        tmp1 = a[0] * a[2] * t;
        tmp2 = a[1] * s;
        m[ 8] = tmp1 - tmp2;
        m[ 2] = tmp1 + tmp2;
        
        tmp1 = a[1] * a[2] * t;
        tmp2 = a[0] * s;
        m[ 9] = tmp1 + tmp2;
        m[ 6] = tmp1 - tmp2;
        
        m[ 3] = m[ 7] = m[11] = 0.0;
        m[12] = m[13] = m[14] = 0.0;
        m[15] = 1.0;
    }
}


Matrix::Matrix(const Vector3f &position, const Vector3f &angles)
{
    float sx = sin(angles[0]);
    float cx = cos(angles[0]);
    float sy = sin(angles[1]);
    float cy = cos(angles[1]);
    float sz = sin(angles[2]);
    float cz = cos(angles[2]);
    
    m[0]  = cy*cz;
    m[1]  = sx*sy*cz + cx*sz;
    m[2]  = -cx*sy*cz + sx*sz;
    m[3]  = 0.0;
    m[4]  = -cy*sz;
    m[5]  = -sx*sy*sz + cx*cz;
    m[6]  = cx*sy*sz + sx*cz;
    m[7]  = 0.0;
    m[8]  = sy;
    m[9]  = -sx*cy;
    m[10] = cx*cy;
    
    m[11] = 0.0;
    
    m[12] = position[0];
    m[13] = position[1];
    m[14] = position[2];
    m[15] = 1;
}

Matrix& Matrix::operator=(const Matrix& other)
{
    if(this != &other)
    {
        for(int i = 0; i < 16; i++)
        {
            m[i] = other.m[i];
        }
        return *this;
    }
    else
    {
        return *this;
    }
}


Matrix Matrix::operator*(const float &scale) const
{
    float new_matrix[16];
    for(int i = 0; i < 16; i++){
        new_matrix[i] = m[i] * scale;
    }
    return Matrix(new_matrix);
}

Matrix Matrix::operator*(const Matrix &other) const
{
    float new_matrix[16];
    new_matrix[ 0] = m[ 0] * other[ 0] + m[ 4] * other[ 1] + m[ 8] * other[ 2] + m[12] * other[ 3];
    new_matrix[ 1] = m[ 1] * other[ 0] + m[ 5] * other[ 1] + m[ 9] * other[ 2] + m[13] * other[ 3];
    new_matrix[ 2] = m[ 2] * other[ 0] + m[ 6] * other[ 1] + m[10] * other[ 2] + m[14] * other[ 3];
    new_matrix[ 3] = m[ 3] * other[ 0] + m[ 7] * other[ 1] + m[11] * other[ 2] + m[15] * other[ 3];
    new_matrix[ 4] = m[ 0] * other[ 4] + m[ 4] * other[ 5] + m[ 8] * other[ 6] + m[12] * other[ 7];
    new_matrix[ 5] = m[ 1] * other[ 4] + m[ 5] * other[ 5] + m[ 9] * other[ 6] + m[13] * other[ 7];
    new_matrix[ 6] = m[ 2] * other[ 4] + m[ 6] * other[ 5] + m[10] * other[ 6] + m[14] * other[ 7];
    new_matrix[ 7] = m[ 3] * other[ 4] + m[ 7] * other[ 5] + m[11] * other[ 6] + m[15] * other[ 7];
    new_matrix[ 8] = m[ 0] * other[ 8] + m[ 4] * other[ 9] + m[ 8] * other[10] + m[12] * other[11];
    new_matrix[ 9] = m[ 1] * other[ 8] + m[ 5] * other[ 9] + m[ 9] * other[10] + m[13] * other[11];
    new_matrix[10] = m[ 2] * other[ 8] + m[ 6] * other[ 9] + m[10] * other[10] + m[14] * other[11];
    new_matrix[11] = m[ 3] * other[ 8] + m[ 7] * other[ 9] + m[11] * other[10] + m[15] * other[11];
    new_matrix[12] = m[ 0] * other[12] + m[ 4] * other[13] + m[ 8] * other[14] + m[12] * other[15];
    new_matrix[13] = m[ 1] * other[12] + m[ 5] * other[13] + m[ 9] * other[14] + m[13] * other[15];
    new_matrix[14] = m[ 2] * other[12] + m[ 6] * other[13] + m[10] * other[14] + m[14] * other[15];
    new_matrix[15] = m[ 3] * other[12] + m[ 7] * other[13] + m[11] * other[14] + m[15] * other[15];
    return Matrix(new_matrix);
}

/**
 * @brief 	Matrix addition operator. Returns a new matrix
 *
 */

Matrix Matrix::operator+(const Matrix &other) const
{
    float new_matrix[16];
    for(int i = 0; i < 16; i++)
    {
        new_matrix[i] = m[i] + other[i];
    }
    return Matrix(new_matrix);
}


Matrix Matrix::operator+=(const Matrix &other)
{
    *this = *this + other;
    return *this;
}

Matrix Matrix::operator*(const float* &other) const
{
    float new_matrix[16];
    new_matrix[ 0] = m[ 0] * other[ 0] + m[ 4] * other[ 1] + m[ 8] * other[ 2] + m[12] * other[ 3];
    new_matrix[ 1] = m[ 1] * other[ 0] + m[ 5] * other[ 1] + m[ 9] * other[ 2] + m[13] * other[ 3];
    new_matrix[ 2] = m[ 2] * other[ 0] + m[ 6] * other[ 1] + m[10] * other[ 2] + m[14] * other[ 3];
    new_matrix[ 3] = m[ 3] * other[ 0] + m[ 7] * other[ 1] + m[11] * other[ 2] + m[15] * other[ 3];
    new_matrix[ 4] = m[ 0] * other[ 4] + m[ 4] * other[ 5] + m[ 8] * other[ 6] + m[12] * other[ 7];
    new_matrix[ 5] = m[ 1] * other[ 4] + m[ 5] * other[ 5] + m[ 9] * other[ 6] + m[13] * other[ 7];
    new_matrix[ 6] = m[ 2] * other[ 4] + m[ 6] * other[ 5] + m[10] * other[ 6] + m[14] * other[ 7];
    new_matrix[ 7] = m[ 3] * other[ 4] + m[ 7] * other[ 5] + m[11] * other[ 6] + m[15] * other[ 7];
    new_matrix[ 8] = m[ 0] * other[ 8] + m[ 4] * other[ 9] + m[ 8] * other[10] + m[12] * other[11];
    new_matrix[ 9] = m[ 1] * other[ 8] + m[ 5] * other[ 9] + m[ 9] * other[10] + m[13] * other[11];
    new_matrix[10] = m[ 2] * other[ 8] + m[ 6] * other[ 9] + m[10] * other[10] + m[14] * other[11];
    new_matrix[11] = m[ 3] * other[ 8] + m[ 7] * other[ 9] + m[11] * other[10] + m[15] * other[11];
    new_matrix[12] = m[ 0] * other[12] + m[ 4] * other[13] + m[ 8] * other[14] + m[12] * other[15];
    new_matrix[13] = m[ 1] * other[12] + m[ 5] * other[13] + m[ 9] * other[14] + m[13] * other[15];
    new_matrix[14] = m[ 2] * other[12] + m[ 6] * other[13] + m[10] * other[14] + m[14] * other[15];
    new_matrix[15] = m[ 3] * other[12] + m[ 7] * other[13] + m[11] * other[14] + m[15] * other[15];
    return Matrix(new_matrix);
}


Vector3f Matrix::operator*(const Vector3f &v) const
{
    float x = m[ 0] * v[0] + m[ 4] * v[1] + m[8 ] * v[2];
    float y = m[ 1] * v[0] + m[ 5] * v[1] + m[9 ] * v[2];
    float z = m[ 2] * v[0] + m[ 6] * v[1] + m[10] * v[2];
    
    x = x + m[12];
    y = y + m[13];
    z = z + m[14];
    
    return Vector3f(x, y, z);
}


/**
 * @brief	Transposes the current matrix
 */
void Matrix::transpose()
{
    float m_tmp[16];
    m_tmp[0]  = m[0];
    m_tmp[4]  = m[1];
    m_tmp[8]  = m[2];
    m_tmp[12] = m[3];
    m_tmp[1]  = m[4];
    m_tmp[5]  = m[5];
    m_tmp[9]  = m[6];
    m_tmp[13] = m[7];
    m_tmp[2]  = m[8];
    m_tmp[6]  = m[9];
    m_tmp[10] = m[10];
    m_tmp[14] = m[11];
    m_tmp[3]  = m[12];
    m_tmp[7]  = m[13];
    m_tmp[11] = m[14];
    m_tmp[15] = m[15];
    for(int i = 0; i < 16; i++) m[i] = m_tmp[i];
}

/**
 * @brief	Computes an Euler representation (x, y, z) plus three
 * 			rotation values in rad. Rotations are with respect to
 * 			the x, y, z axes.
 */
void Matrix::toPostionAngle(float pose[6])
{
    if(pose != 0){
        float _trX, _trY;
        if(m[0] > 0.0) {
            pose[4] = asin(m[8]);
        } else {
            pose[4] = (float)M_PI - asin(m[8]);
        }
        // rPosTheta[1] =  asin( m[8]);      // Calculate Y-axis angle
        
        float  C    =  cos( pose[4] );
        if ( fabs( C ) > 0.005 )  {          // Gimball lock?
            _trX      =  m[10] / C;          // No, so get X-axis angle
            _trY      =  -m[9] / C;
            pose[3]  = atan2( _trY, _trX );
            _trX      =  m[0] / C;           // Get Z-axis angle
            _trY      = -m[4] / C;
            pose[5]  = atan2( _trY, _trX );
        } else {                             // Gimball lock has occurred
            pose[3] = 0.0;                   // Set X-axis angle to zero
            _trX      =  m[5];  //1          // And calculate Z-axis angle
            _trY      =  m[1];  //2
            pose[5]  = atan2( _trY, _trX );
        }
        
        pose[0] = m[12];
        pose[1] = m[13];
        pose[2] = m[14];
    }
}


/**
 * @brief	Matrix scaling with self assignment.
 */

void Matrix::operator*=(const float& scale)
{
    *this = *this * scale;
}

/**
 * @brief 	Matrix-Matrix multiplication with self assigment.
 */

void Matrix::operator*=(const Matrix& other)
{
    *this = *this * other;
}

/**
 * @brief	Matrix-Matrix multiplication (array based). See \ref{operator*}.
 */

void Matrix::operator*=(const float* other)
{
    *this = *this * other;
}

/**
 * @brief	Returns the internal data array. Unsafe. Will probably
 * 			removed in one of the next versions.
 */
float* Matrix::getData(){ return m;};


/**
 * @brief	Indexed element (reading) access.
 */
float Matrix::operator[](const int& index) const
{
    /// TODO: Boundary check
    return m[index];
}


/**
 * @brief  	Writeable index access
 */
float& Matrix::operator[](const int& index)
{
    return m[index];
}

/**
 * @brief   Returns the matrix's determinant
 */
float Matrix::det()
{
    float det, result = 0, i = 1.0;
    float Msub3[9];
    int    n;
    for ( n = 0; n < 4; n++, i *= -1.0 ) {
        submat( Msub3, 0, n );
        det     = det3( Msub3 );
        result += m[n] * det * i;
    }
    return( result );
}

Matrix Matrix::inv(bool& success)
{
    Matrix Mout;
    float  mdet = det();
    if ( fabs( mdet ) < 0.00000000000005 ) {
        cout << "Error matrix inverting! " << mdet << endl;
        return Mout;
    }
    float  mtemp[9];
    int     i, j, sign;
    for ( i = 0; i < 4; i++ ) {
        for ( j = 0; j < 4; j++ ) {
            sign = 1 - ( (i +j) % 2 ) * 2;
            submat( mtemp, i, j );
            Mout[i+j*4] = ( det3( mtemp ) * sign ) / mdet;
        }
    }
    return Mout;
}


/**
 * @brief   Returns a sub matrix without row \ref i and column \ref j.
 */
void Matrix::submat(float* submat, const int& i, const int& j)
{
    int di, dj, si, sj;
    // loop through 3x3 submatrix
    for( di = 0; di < 3; di ++ ) {
        for( dj = 0; dj < 3; dj ++ ) {
            // map 3x3 element (destination) to 4x4 element (source)
            si = di + ( ( di >= i ) ? 1 : 0 );
            sj = dj + ( ( dj >= j ) ? 1 : 0 );
            // copy element
            submat[di * 3 + dj] = m[si * 4 + sj];
        }
    }
}

/**
 * @brief    Calculates the determinant of a 3x3 matrix
 *
 * @param    M  input 3x3 matrix
 * @return   determinant of input matrix
 */
float Matrix::det3( const float *M )
{
    float det;
    det = (double)(  M[0] * ( M[4]*M[8] - M[7]*M[5] )
                   - M[1] * ( M[3]*M[8] - M[6]*M[5] )
                   + M[2] * ( M[3]*M[7] - M[6]*M[4] ));
    return ( det );
}
    
} // namespace asteroids

