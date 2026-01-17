#include <QList>
#include <QtAlgorithms>

// #include <new>

// #include "../elements/element.h"
// #include "../elements/elementprovider.h"
// #include "../elements/load/load.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/displacement/displacement.h"
#include "mesh.h"

bool Mesh::isEqual(Point3 p1, Point3 p2) {
  return p1.x - p2.x < 0.001 && p1.y - p2.y < 0.001 && p1.y - p2.y < 0.001;
}

unsigned Mesh::maxNodeIndexInList(const QList<Node> &list) {
  if (list.empty())
    throw std::exception();

  int maxIndex = list.first().id;
  for (auto item : list) {
    if (item.id > maxIndex)
      maxIndex = item.id;
  }
  return maxIndex;
}

// template <ElementType type> void Mesh::createDefaultMesh() {
//   // ElementProvider::initialize();

//   auto data = ElementProvider<type>::data;
//   AreaLoadPlate load = AreaLoadPlate(-100);

//   float startx = 0;
//   float starty = 0;
//   float startz = 0;
//   Point3 point00{startx, starty, startz};

//   float step = 100;
//   float lz = 1000;
//   float lenghtPlate = 3000; // В мм
//   int steps = (int)(lenghtPlate / step);
//   int elementCount = lenghtPlate * lenghtPlate / (step * step);

//   // Allocation for element array
//   void *elementMemory = operator new(sizeof(AbstractElement) * elementCount);

//   elements.reserve(elementCount);
//   nodes.reserve(elementCount);

//   float sinA = 0;
//   float cosA = 1;

//   for (int l = 1; l <= steps; l++) {
//     for (int k = 1; k <= steps; k++) {

//       Point3 point0{point00.x + l * step * cosA, point00.y + k * step,
//                     point00.x + l * step * sinA};
//       Node node;
//       const int count = data;
//       Node nodesToElem[count];
//       float checkValue = 0.01f;

//       int elementsCount = 0;

//       for (int j = 0; j < count; j++) {

//         Point3 pointForNode =
//             data.GET_POINT_FROM_INDEX_FN(j, point0, step, cosA, sinA);

//         Node possibleNode;
//         bool flag = false;
//         if (j != 0) {
//           // Проверка на то есть в этой точке уже нод или нет
//           for (auto item : this->nodes) {
//             if (isEqual(item.point, pointForNode)) {
//               possibleNode = item;
//               flag = true;
//               break;
//             }
//           }
//         }

//         if (flag) {
//           node =
//               Node(possibleNode.point, possibleNode.dofCount,
//               possibleNode.id);
//         } else {
//           int index = 0;

//           if (j != 0) {
//             index = maxNodeIndexInList(this->nodes) + 1;
//           }

//           node = Node(pointForNode, data.DOF_COUNT, index);

//           // Load standart sheme add
//           // if (node.point.y == starty)
//           //   node.nodeLoad = &load;

//           // Add displ
//           if (node.point.y == starty || node.point.y == starty + lenghtPlate)
//           {
//             bool values[3] = {true, true, true};
//             node.nodeDisplacement =
//                 NodeDisplacement::create(DisplType::UzRxRy, values);
//           }
//         }

//         nodesToElem[j] = node;
//         this->nodes.push_back(node);
//       }

//       void *ptr = static_cast<char *>(elementMemory) +
//                   elementCount * sizeof(AbstractElement);

//       this->elements.push_back(AbstractElement::create(
//           elementsCount, type, nodesToElem, data.NODES_COUNT, ptr));
//       elementsCount++;
//     }
//   }
// }

// template <> void Mesh::createDefaultMesh<ElementType::MITC4MY>() {}