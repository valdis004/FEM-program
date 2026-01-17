#pragma once

enum ElementType { MITC4MY = 0, MITC9 = 1, MITC16 = 2, NONE = 3 };

enum DisplType // Тип степеней свободы, который поддерживает узел
{
  Ux,
  Uy,
  Uz,
  UxUy,
  RUy,
  UxUyR,
  UzRxRy,
  RxRy
};

enum LoadType {
  Fx,
  Fy,
  FxFy,
  MFy,
  MFx,
  MFxy,
  q,
  FyMxMz,
  MxMz,
};

// enum OutputType {
//   Ux = 0,
//   Uy = 1,
//   Theta = 2,
//   Mx = 3,
//   Qx = 4,
//   Nx = 5,
//   My = 6,

//   Mxy = 7,
//   Qy = 8,
//   None = 9
// };

//