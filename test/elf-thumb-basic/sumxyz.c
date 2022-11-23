
int sumxyz(int x, int y, int z)
{
    int ans0 = sumxy(x, y);
    int ans1 = sumxy(ans0, z);
    return ans1;
}
