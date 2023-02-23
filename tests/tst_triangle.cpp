#include <QtTest>

#include "triangle.hpp"
#include <QList>
#include <QMap>

class test_triangle : public QObject {
  Q_OBJECT

public:
  test_triangle();
  ~test_triangle();

private slots:
  void test_fronts_validation();
  void test_angles_validation();
  void test_3fronts_calculating();
  void test_angles_filling();
  void test_2fronts_triangle();
  void test_rectangular_triangles();
  void test_equileterial_triangles();
  void test_calc_square();
};

test_triangle::test_triangle() {}

test_triangle::~test_triangle() {}

void test_triangle::test_fronts_validation() {
  QVERIFY(not Triangle().validFronts({{0, 30}, {1, 15}, {2, 15}}));
  QVERIFY(not Triangle().validFronts({{0, 30}, {1, 60}, {2, 30}}));
  QVERIFY(not Triangle().validFronts({{0, 50}, {1, 50}, {2, 100}}));
  QVERIFY(Triangle().validFronts({{0, 12}, {1, 14}, {2, 13}}));
  QVERIFY(not Triangle().validFronts({{0, 20}, {1, 20}, {2, 40}}));
  QVERIFY(Triangle({{{0, 10}, {1, 10}, {2, 10}}}, {}).isValidTriangle());
  QVERIFY(not Triangle({{{0, 10.1}, {1, 10}, {2, 10}}},
                       {{0, 60}, {1, 60}, {2, 60}}, 1)
                  .isValidTriangle());
  QVERIFY(
      not Triangle({{{0, 10.5}, {1, 10}, {2, 10}}}, {{0, 60}, {1, 60}, {2, 60}})
              .isValidTriangle());
  QVERIFY(not Triangle({{{0, 10}, {1, 10}, {2, 10}}},
                       {{0, 59.5}, {1, 60.5}, {2, 60}})
                  .isValidTriangle());
}

void test_triangle::test_angles_validation() {
  QVERIFY(not Triangle().validAvailableAngles({{1, 180}}));
  QVERIFY(not Triangle().validAvailableAngles({{0, 90}, {1, 90}}));
  QVERIFY(not Triangle().validAvailableAngles({{1, 90}, {2, 100}}));
  QVERIFY(Triangle().validAvailableAngles({{0, 30}, {1, 90}, {2, 60}}));
}

void test_triangle::test_3fronts_calculating() {
  // Test arbirary triangles
  QCOMPARE(Triangle({{0, 3}, {1, 4}, {2, 5}}, {}).anglesAsMap().values(),
           QList<double>({37, 53, 90}));
  QCOMPARE(Triangle({{0, 37}, {1, 40}, {2, 54}}, {}).anglesAsMap().values(),
           QList<double>({43, 48, 89}));

  // Test equilateral triangle
  QCOMPARE(Triangle({{0, 10}, {1, 10}, {2, 10}}, {}).anglesAsMap().values(),
           QList<double>({60, 60, 60}));

  // Test unvalid cases (some angle doens't match or 2 angles >= 90)
  QCOMPARE(Triangle({{0, 10}, {1, 10}, {2, 10}}, {{0, 90}, {1, 90}})
               .anglesAsMap()
               .values(),
           QList<double>({90, 90}));

  QCOMPARE(
      Triangle({{0, 10}, {1, 10}, {2, 10}}, {{0, 90}}).anglesAsMap().values(),
      QList<double>({90}));
}

void test_triangle::test_angles_filling() {
  // correct triangles
  QCOMPARE(Triangle({{0, 8}, {1, 8}, {2, 8}}, {{0, 60}, {2, 60}})
               .anglesAsMap()
               .values(),
           QList<double>({60, 60, 60}));
  // uncorrect angles
  QCOMPARE(Triangle({{0, 43}, {1, 44}, {2, 42}}, {{1, 90}, {2, 90}})
               .anglesAsMap()
               .values(),
           QList<double>({90, 90}));
  // uncorrect fronts
  QCOMPARE(Triangle({{0, 20}, {1, 40}, {2, 20}}, {{1, 30}, {2, 90}})
               .anglesAsMap()
               .values(),
           QList<double>({30, 90}));
  // with 1 known angle
  QCOMPARE(Triangle({{0, 3}, {1, 4}, {2, 5}}, {{2, 90}}).anglesAsMap().values(),
           QList<double>({37, 53, 90}));
  // test with a wrong angle
  QCOMPARE(
      Triangle({{0, 37}, {1, 40}, {2, 54}}, {{0, 48}}).anglesAsMap().values(),
      QList<double>({48}));
}

void test_triangle::test_2fronts_triangle() {
  // fronts: 89, (73), 85
  // angles: 68.07, 49.55, 62.38
  QCOMPARE(Triangle({{0, 89}, {2, 85}}, {{0, 68.07}, {1, 49.55}, {2, 62.38}}, 2)
               .frontsAsMap()
               .values(),
           QList<double>({89, 73.01, 85}));
  // fronts: (3), 4, 5
  // angles: 37, 53, 90
  QCOMPARE(Triangle({{1, 4}, {2, 5}}, {{0, 37}, {1, 53}, {2, 90}})
               .frontsAsMap()
               .values(),
           QList<double>({3, 4, 5}));
  // fronts: 8, 9, (10)
  // angles: 49, 59, 72
  QCOMPARE(Triangle({{0, 80}, {1, 90}}, {{0, 49}, {1, 59}, {2, 72}})
               .frontsAsMap()
               .values(),
           QList<double>({80, 90, 100}));
  // fronts: (73), 80, 82
  // angles: 53.6, 61.8, 64.6
  QCOMPARE(Triangle({{1, 80}, {2, 82}}, {{0, 53.6}, {1, 61.8}, {2, 64.6}}, 2)
               .frontsAsMap()
               .values(),
           QList<double>({73.06, 80, 82}));

  // fronts: 93, 67, (89)
  // angles: 72, (43), 65
  QCOMPARE(
      Triangle({{0, 93}, {1, 67}}, {{0, 72}, {2, 65}}).frontsAsMap().values(),
      QList<double>({93, 67, 89}));
  QCOMPARE(
      Triangle({{0, 93}, {1, 67}}, {{0, 72}, {2, 65}}).anglesAsMap().values(),
      QList<double>({72, 43, 65}));

  // fronts: 435, 500, 378
  // angles: 57, (76), 47
  QCOMPARE(
      Triangle({{1, 500}, {2, 378}}, {{0, 57}, {2, 47}}).frontsAsMap().values(),
      QList<double>({432, 500, 378}));
  QCOMPARE(
      Triangle({{1, 500}, {2, 378}}, {{0, 57}, {2, 47}}).anglesAsMap().values(),
      QList<double>({57, 76, 47}));

  // fronts: 19, (15), 13
  // angles: 85, 52, 43
  QCOMPARE(
      Triangle({{0, 19}, {2, 13}}, {{1, 52}, {2, 43}}).frontsAsMap().values(),
      QList<double>({19, 15, 13}));
  QCOMPARE(
      Triangle({{0, 19}, {2, 13}}, {{1, 52}, {2, 43}}).anglesAsMap().values(),
      QList<double>({85, 52, 43}));

  // fronts: 4, 5, (7)
  // angles: (34), (44), 102
  QCOMPARE(Triangle({{0, 4}, {1, 5}}, {{2, 102}}).frontsAsMap().values(),
           QList<double>({4, 5, 7}));
  QCOMPARE(Triangle({{0, 4}, {1, 5}}, {{2, 102}}).anglesAsMap().values(),
           QList<double>({34, 44, 102}));
}

void test_triangle::test_rectangular_triangles() {
  // fronts: 10, (5), (9)
  // angles: 90, 30, (60)
  QCOMPARE(Triangle({{0, 10}}, {{0, 90}, {1, 30}}).frontsAsMap().values(),
           QList<double>({10, 5, 8.66}));
  QCOMPARE(Triangle({{0, 10}}, {{0, 90}, {1, 30}}).anglesAsMap().values(),
           QList<double>({90, 30, 60}));

  // fronts: (nan), 18, 7
  // angles: * * 90
  QVERIFY(not std::isnan(
      Triangle({{1, 18}, {2, 7}}, {{2, 90}}).frontsAsMap().values()[0]));
}

void test_triangle::test_equileterial_triangles() {
  // fronts: 10, (10), (10)
  // angles: 60, 60, (60)
  QCOMPARE(Triangle({{0, 10}}, {{0, 60}, {1, 60}}).frontsAsMap().values(),
           QList<double>({10, 10, 10}));
  QCOMPARE(Triangle({{0, 10}}, {{0, 60}, {1, 60}}).anglesAsMap().values(),
           QList<double>({60, 60, 60}));

  // fronts: 9, (5), (9)
  // angles: 74, 32, 74
  QCOMPARE(
      Triangle({{0, 9}}, {{0, 74}, {1, 32}, {2, 74}}).frontsAsMap().values(),
      QList<double>({9, 5, 9}));
  QCOMPARE(
      Triangle({{0, 9}}, {{0, 74}, {1, 32}, {2, 74}}).anglesAsMap().values(),
      QList<double>({74, 32, 74}));
}

void test_triangle::test_calc_square() {
  // fronts: 3, 4, 5
  // square: 6
  QCOMPARE(Triangle({{0, 3}, {1, 4}, {2, 5}}, {}).square, 6);
  // fronts: 23, 17, 15
  // square: ~127
  QCOMPARE(Triangle({{0, 23}, {1, 17}, {2, 15}}, {}).square, 127);
}

QTEST_APPLESS_MAIN(test_triangle)

#include "tst_triangle.moc"
