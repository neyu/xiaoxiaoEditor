//
//  csUtils.h
//  test
//
//  Created by coolsoft on 15/1/31.
//
//

#ifndef __test__csUtils__
#define __test__csUtils__

#include "csAppClient.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//#define MAXFLOAT 0x1.fffffep+127f
const float MAXFLOAT = (std::numeric_limits<float>::max)();
const float MINFLOAT = (std::numeric_limits<float>::min)();
#endif


#define random_range(a, b) ((rand() % ((b) - (a) + 1)) + (a))
#define frandom ((float)rand() / (float)(RAND_MAX))
#define frandom_range(a, b) (((b - a) * frandom) + a)

#ifndef PI
#define PI 3.1415926
#endif
#define PI2 6.283185307179586
#define PI256 804.247719318987

inline float interpolate(float a, float b, float v)
{
    return a + (b - a) * v;
}
inline double interpolate(double a, double b, float v)
{
    return a + (b - a) * v;
}

inline Vec2 polar(float radius, float radian)
{
    float x = radius * cosf(radian);
    float y = radius * sinf(radian);
    return Vec2(x, y);
}

inline Vec2 interpolate2D(Vec2 fromPosition, Vec2 toPosition, float mv)
{
    Vec2 ret;
    ret.x = interpolate(fromPosition.x, toPosition.x, mv);
    ret.y = interpolate(fromPosition.y, toPosition.y, mv);
    return ret;
}

inline Vec3 interpolate3D(Vec3 fromPosition, Vec3 toPosition, float mv)
{
    Vec3 ret;
    ret.x = interpolate(fromPosition.x, toPosition.x, mv);
    ret.y = interpolate(fromPosition.y, toPosition.y, mv);
    ret.z = interpolate(fromPosition.z, toPosition.z, mv);
    return ret;
}

inline float uninterpolate(float a, float b, float v)
{
    if(a == v) return 0;
    return (v-a)/(b-a);
}

template<class T>
inline int indexOf(vector<T> container, T element)
{
    int ret = -1;
    for (int i=0; i < container.size(); ++i)
    {
        if (element == container[i])
        {
            ret = i;
            break;
        }
    }
    return ret;
}

inline int random(int a)
{
    return rand()%a;
}
inline float random(float a)
{
    return (a * frandom);
}
inline float randomFloatAt(float a, float b)
{
    return (((b - a) * frandom) + a);
}

inline int randomAt(int a, int b)
{
    return ((rand() % (abs(b - a) + 1)) + MIN(a,b));
}

inline float getBezierNumber(float a, float b, float c, float v)
{
    float n = 1.0-v;
    return a*n*n+2*b*n*v+c*v*v;
}
inline Vec2 getBezierPoint(Vec2 a, Vec2 b, Vec2 c, float v)
{
    return Vec2(getBezierNumber(a.x,b.x,c.x,v),getBezierNumber(a.y,b.y,c.y,v));
}

inline vector<Vec2> getBezlerSegment(Vec2 c, Vec2 b, Vec2 a, float v1, float v2)
{
    Vec2 ab1 = interpolate2D(b, a, v1);
    Vec2 ab2 = interpolate2D(b, a, v2);
    Vec2 bc1 = interpolate2D(c, b, v1);
    Vec2 bc2 = interpolate2D(c, b, v2);
    Vec2 aa = interpolate2D(bc1, ab1, v1);
    Vec2 cc = interpolate2D(bc2, ab2, v2);
    Vec2 bb = interpolate2D(bc1, ab1, v2);
    
    vector<Vec2> vecRet;
    vecRet.push_back(aa);
    vecRet.push_back(bb);
    vecRet.push_back(cc);
    return vecRet;
}

inline float getBezierLength(Vec2 startPoint, Vec2 controlPoint, Vec2 endPoint)
{
    float time = 1.0f;
    const float PRECISION = 1e-10;
    const float csX = controlPoint.x - startPoint.x;
    const float csY = controlPoint.y - startPoint.y;
    const float nvX = endPoint.x - controlPoint.x - csX;
    const float nvY = endPoint.y - controlPoint.y - csY;
    
    // vectors: c0 = 4*(cs,cs), с1 = 8*(cs, ec-cs), c2 = 4*(ec-cs,ec-cs)
    const float c0 = 4 * (csX * csX + csY * csY);
    const float c1 = 8 * (csX * nvX + csY * nvY);
    const float c2 = 4 * (nvX * nvX + nvY * nvY);
    
    float ft;
    float f0;
    
    if (c2 == 0) {
        if (c1 == 0) {
            ft = sqrt(c0) * time;
            return ft;
        } else {
            ft = (2 / 3) * (c1 * time + c0) * sqrt(c1 * time + c0) / c1;
            f0 = (2 / 3) * c0 * sqrt(c0) / c1;
            return (ft - f0);
        }
    } else {
        const float sqrt_0 = sqrt(c2 * time * time + c1 * time + c0);
        const float sqrt_c0 = sqrt(c0);
        const float sqrt_c2 = sqrt(c2);
        const float exp1 = (0.5 * c1 + c2 * time) / sqrt_c2 + sqrt_0;
        
        if (exp1 < PRECISION) {
            ft = 0.25 * (2 * c2 * time + c1) * sqrt_0 / c2;
        } else {
            ft = 0.25 * (2 * c2 * time + c1) * sqrt_0 / c2 + 0.5 * logf((0.5 * c1 + c2 * time) / sqrt_c2 + sqrt_0) / sqrt_c2 * (c0 - 0.25 * c1 * c1 / c2);
        }
        
        const float exp2 = (0.5 * c1) / sqrt_c2 + sqrt_c0;
        if (exp2 < PRECISION) {
            f0 = 0.25 * (c1) * sqrt_c0 / c2;
        } else {
            f0 = 0.25 * (c1) * sqrt_c0 / c2 + 0.5 * logf((0.5 * c1) / sqrt_c2 + sqrt_c0) / sqrt_c2 * (c0 - 0.25 * c1 * c1 / c2);
        }
        return ft - f0;
    }
}

inline Vec2 beelineIntersectPoint(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4, bool isInternal=false)
{
    Vec2 P1 = p1;
    Vec2 P2 = p2;
    Vec2 Q1 = p3;
    Vec2 Q2 = p4;
    float x, y;
    float xV;
    float yV;
    if (P1.y == P2.y)
    {
        if(Q1.y == Q2.y) return Vec2::ZERO;//null;
        y = P1.y;
        if(Q2.x == Q1.x)
        {
            x = Q1.x;
        }
        else
        {
            x = (P1.y - Q1.y) / (Q2.y - Q1.y) * (Q2.x - Q1.x) + Q1.x;
        }
        if(isInternal)
        {
            xV = (x - P1.x)/(P2.x - P1.x);
            yV = (y - Q1.y)/(Q2.y - Q1.y);
            if(xV >= 0 && xV < 1 && yV >= 0 && yV < 1)
            {
                return Vec2(x, y);
            }
            else
            {
                return Vec2::ZERO;//null
            }
        }
        return Vec2(x, y);
    }
    if (Q1.y == Q2.y)
    {
        if(P1.y == P2.y) return Vec2::ZERO;// null;
        y = Q1.y;
        if(P2.x == P1.x)
        {
            x = P1.x;
        }
        else
        {
            x = (Q1.y - P1.y) / (P2.y - P1.y) * (P2.x - P1.x) + P1.x;
        }
        if(isInternal)
        {
            xV = (x - Q1.x)/(Q2.x - Q1.x);
            yV = (y - P1.y)/(P2.y - P1.y);
            if(xV >= 0 && xV < 1 && yV >= 0 && yV < 1)
            {
                return Vec2(x, y);
            }
            else
            {
                return Vec2::ZERO;// null
            }
        }
        return Vec2(x, y);
    }
    //
    if (P1.x == P2.x)
    {
        if(Q1.x == Q2.x) return Vec2::ZERO;// null;
        x = P1.x;
        if(Q2.y == Q1.y)
        {
            y = Q1.y;
        }
        else
        {
            y = (P1.x - Q1.x) / (Q2.x - Q1.x) * (Q2.y - Q1.y) + Q1.y;
        }
        if(isInternal)
        {
            xV = (x - Q1.x)/(Q2.x - Q1.x);
            yV = (y - P1.y)/(P2.y - P1.y);
            if(xV >= 0 && xV < 1 && yV >= 0 && yV < 1)
            {
                return Vec2(x, y);
            }
            else
            {
                return Vec2::ZERO;// null
            }
        }
        return Vec2(x, y);
    }
    if (Q1.x == Q2.x)
    {
        if(P1.x == P2.x) return Vec2::ZERO;// null;
        x = Q1.x;
        if(P2.y == P1.y)
        {
            y = P1.y;
        }
        else
        {
            y = (Q1.x - P1.x) / (P2.x - P1.x) * (P2.y - P1.y) + P1.y;
        }
        if(isInternal)
        {
            xV = (x - P1.x)/(P2.x - P1.x);
            yV = (y - Q1.y)/(Q2.y - Q1.y);
            if(xV >= 0 && xV < 1 && yV >= 0 && yV < 1)
            {
                return Vec2(x, y);
            }
            else
            {
                return Vec2::ZERO;// null
            }
        }
        return Vec2(x, y);
    }
    float k1 = (P1.y - P2.y) / (P1.x - P2.x);
    float n1 = P1.y / k1 - P1.x;
    float k2 = (Q1.y - Q2.y) / (Q1.x - Q2.x);
    float n2 = Q1.y / k2 - Q1.x;
    float xx = (k2 * n2 - k1 * n1) / (k1 - k2);
    x = xx;
    y = k1 * (xx + n1);
    if(isInternal)
    {
        xV = (x - Q1.x)/(Q2.x - Q1.x);
        yV = (y - Q1.y)/(Q2.y - Q1.y);
        if(xV >= 0 && xV < 1 && yV >= 0 && yV < 1)
        {
            xV = (x - P1.x)/(P2.x - P1.x);
            yV = (y - P1.y)/(P2.y - P1.y);
            if(xV >= 0 && xV < 1 && yV >= 0 && yV < 1)
            {
                return Vec2(x, y);
            }
            else
            {
                return Vec2::ZERO;// null;
            }
        }
        else
        {
            return Vec2::ZERO;// null
        }
    }
    return Vec2(x, y);
}
template<typename T>
void PushBack(vector<T>& myVector, int len, ...)
{
    va_list pvar;
    va_start(pvar, len);
    for (int j=0; j < len; j++)
    {
        T param = va_arg(pvar, T);
        myVector.push_back(param);
    }
    va_end(pvar);
}

inline float equation2x(float a, float b, float c, bool addition)
{
    //axx+bx+c=0
    //		x=[-b±√(b^2-4ac)]/2a
    return addition ? (-b+sqrt(b*b-4*a*c))/(2*a) : (-b-sqrt(b*b-4*a*c))/(2*a);
}
//MARK:
inline vector<float> getParabolaTimeWithY(float y, float fromY, float speedY, float gravityY)
{
    vector<float> arr;
    arr.push_back(equation2x(-gravityY,-speedY-gravityY,y-fromY,true));
    arr.push_back(equation2x(-gravityY,-speedY-gravityY,y-fromY,false));
    return arr;
}
inline float getParabolaTopTime(float speedY, float gravityY)
{
    return -(speedY+gravityY)/gravityY/2;
}
inline float getParabolaHeight(float speedY, float gravityY)
{
    float a = speedY+gravityY;
    float t = -a/gravityY/2;
    return -t*a-t*t*gravityY;
}
inline float getParabolaSpeedY(float height, float gravityY)
{
    return -sqrt(height*gravityY*4)-gravityY;
}
//MARK:--------------
inline float modfx(float a, float b)
{
//    int s = (int)(a/b);
    return a-floor(a/b)*b;
}
inline float radianLength(float fromR, float toR,bool isAbs)
{
    if(isAbs) return modfx(toR - fromR + PI256, PI2);
    return modfx(toR - fromR + PI256 + PI,PI2)-PI;
}
inline float getToRotation(float start, float end, bool isCW, int loop)
{
    if(isCW)
    {
        return start + radianLength(start/180.0*PI,end/180.0*PI,true)/PI*180.0+360.0*loop;
    }
    return start - radianLength(end/180.0*PI,start/180.0*PI,true)/PI*180.0-360.0*loop;
}
inline Vec2 anglePoint(Vec2 p,float _cos,float _sin,Vec2 axes)
{
    Vec2 pp = Vec2(0,0);
    float ax = (p.x-axes.x)*_cos-(p.y-axes.y)*_sin;
    pp.y = (p.y-axes.y)*_cos+(p.x-axes.x)*_sin+axes.y;
    pp.x = ax+axes.x;
    return pp;
}
inline Mat4 colorToMat4(const Vec4& c)
{
    return Mat4(c.x,0,0,0,
                0,c.y,0,0,
                0,0,c.z,0,
                0,0,0,c.w);
}

//this function is the same with split()
inline void split_2(string src, const char* token, vector<std::string>& vect)
{
    int nend=0;
    int nbegin=0;
    while(nend != -1)
    {
        nend = src.find(token, nbegin);
        if(nend == -1)
            vect.push_back(src.substr(nbegin, src.length()-nbegin));
        else
            vect.push_back(src.substr(nbegin, nend-nbegin));
        nbegin = nend + strlen(token);
    }
}

// this function is the same with dfsFolder
inline void dfsFolder_2(set<string>& setPath, string folderPath, int depth=0)
{
#ifdef WIN32
    _finddata_t FileInfo;
    string strfind = folderPath + "\\*";
    long Handle = _findfirst(strfind.c_str(), &FileInfo);

    if (Handle == -1L)
    {
        CCLOG("can not match the folder path");
        exit(-1);
    }
    do{
        /*判断是否有子目录*/
        if (FileInfo.attrib & _A_SUBDIR)    
        {
            /*这个语句很重要*/
            if( (strcmp(FileInfo.name,".") != 0 )
                &&(strcmp(FileInfo.name,"..") != 0))   
            {
                string newFilePath = folderPath + "\\" + FileInfo.name;
                dfsFolder_2(setPath, newFilePath);

                string newPath = folderPath + string("\\") + string(FileInfo.name);
                setPath.insert(newPath);
            }
        }
        else  
        {
            string filename = (folderPath + "\\" + FileInfo.name);
            CCLOG("%s\\%s ", folderPath.c_str(), FileInfo.name);
        }
    }while (_findnext(Handle, &FileInfo) == 0);

    _findclose(Handle);
#else
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(folderPath.c_str())) == NULL)
    {
        CCLOG("cannot open directory: %s\n", folderPath.c_str());
        return;
    }
    chdir(folderPath.c_str());
    while((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode))
        {
            if(strcmp(".",entry->d_name) == 0 ||
                strcmp("..",entry->d_name) == 0)
            {
                continue;
            }
            CCLOG("%*s%s/\n",depth,"",entry->d_name);

            string newPath = folderPath + string("/") + entry->d_name;
            setPath.insert(newPath);

            dfsFolder_2(setPath, newPath, depth+4);
        }
        else
        {
            string filename = entry->d_name;
            CCLOG("%*s%s\n",depth,"",entry->d_name);
        }
    }
    chdir("..");
    closedir(dp);
#endif
}

inline int64 s16toi(char* s)
{
    int i,t;
    long sum=0;
    for(i=0;s[i];i++)
    {
        if(s[i]<='9')t=s[i]-'0';
        else if(s[i]>='a') t=s[i]-'a'+10;
        else t=s[i]-'A'+10;
        sum=sum*16+t;
    }
    return sum;
}
inline Color4B s16toColor4(string s)
{
    char buffer[16];
    sprintf(buffer, "%s",s.c_str());
    int64 n = s16toi(buffer);
    return Color4B(n >> 24,n >> 16 & 255,n >> 8 & 255,n & 255);
}
inline Color3B s16toColor3(string s)
{
    char buffer[16];
    sprintf(buffer, "%s",s.c_str());
    int64 n = s16toi(buffer);
    return Color3B((n >> 16) & 255,(n >> 8) & 255,n & 255);
}

inline string intToString(int value)
{
    char charBuffer[32] = {0};
    sprintf(charBuffer, "%d",value);
    return string(charBuffer);
}
inline string floatToString(float value)
{
    char charBuffer[32] = {0};
    sprintf(charBuffer, "%f",value);
    return string(charBuffer);
}
inline float makeGamma(float xb, float xg, float xw)
{
    if (xg == xb) {
        xb = xb - 0.000001;
        xw = xw + 0.000001;
    }
    return log((xg - xb) / (xw - xb)) / log(0.5);
}


#endif /* defined(__test__csUtils__) */
