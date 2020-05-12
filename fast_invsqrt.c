float Q_rsqrt( float number )
{
  long i;
	float x2, y;
  FloatU iface;
	const float threehalfs = 1.5F;

  /* Reinterpret to int */
  iface.f = number;
  i = iface.i >> 1;
  i = 0x5f3759df -  i;

  /* Reinterpret back */
  iface.i = i;
  y = iface.f;

  /* Refine */
  y = y + 0.5f * y * (1 - y * y * number);
  y = y + 0.5f * y * (1 - y * y * number);

	return y;
}