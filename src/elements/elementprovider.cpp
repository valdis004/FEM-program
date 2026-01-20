#include "elementprovider.h"
#include "femtypes.h"
#include <qglobal.h>

/* static */ void ElementProvider::init() {
  struct ElementData MITC4 = {
      4,
      4,
      12,
      true,
      3,
      -1,
      -1,
      QVector<double>{-1.0 / SQRT_3, 1.0 / SQRT_3},
      QVector<double>{-1.0 / SQRT_3, 1.0 / SQRT_3},
      QVector<double>{1.0, 1.0, 1.0, 1.0},
      QVector<short>{3, 3, 3, 3},
      QVector<short>{0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3},
      QVector<short>{},
      QVector<LoadType>{LoadType::FyMxMz, LoadType::FyMxMz, LoadType::FyMxMz,
                        LoadType::FyMxMz},

      QVector<MethodPtrDisp>{
          &NodeDisplacementUzPsixPsiy::create,
          &NodeDisplacementUzPsixPsiy::create,
          &NodeDisplacementUzPsixPsiy::create,
          &NodeDisplacementUzPsixPsiy::create,
      },

      QVector<MethodPtrLoad>{
          &NodeLoadFzMxMy::create,
          &NodeLoadFzMxMy::create,
          &NodeLoadFzMxMy::create,
          &NodeLoadFzMxMy::create,
      },

      [](int index, Point3 &point0, double step, double cosA, double sinA) {
        float x1 = point0.x;
        float y1 = point0.y;
        float z1 = point0.z;

        switch (index) {
        case 0:
          return Point3(x1, y1, z1);
        case 1:
          return Point3(x1 + step * cosA, y1, z1 + step * sinA);
        case 2:
          return Point3(x1 + step * cosA, y1 + step, z1 + step * sinA);
        case 3:
          return Point3(x1, y1 + step, z1);
        default:
          throw std::runtime_error("Invalid index");
        }
      }};
  elementData.insert(ElementType::MITC4MY, MITC4);
}

QMap<ElementType, ElementData> ElementProvider::elementData;