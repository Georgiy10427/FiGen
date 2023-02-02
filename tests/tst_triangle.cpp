#include <QtTest>

#include <QMap>
#include <QList>
#include "triangle.hpp"

class test_triangle : public QObject
{
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
};

test_triangle::test_triangle()
{

}

test_triangle::~test_triangle()
{

}

void test_triangle::test_fronts_validation()
{
    QVERIFY(not Triangle().validFronts({{0, 30}, {1, 15}, {2, 15}}));
    QVERIFY(not Triangle().validFronts({{0, 30}, {1, 60}, {2, 30}}));
    QVERIFY(not Triangle().validFronts({{0, 50}, {1, 50}, {2, 100}}));
    QVERIFY(Triangle().validFronts({{0, 12}, {1, 14}, {2, 13}}));
}

void test_triangle::test_angles_validation()
{
    QVERIFY(not Triangle().validAvailableAngles({{1, 180}}));
    QVERIFY(not Triangle().validAvailableAngles({{0, 90}, {1, 90}}));
    QVERIFY(not Triangle().validAvailableAngles({{1, 90}, {2, 100}}));
    QVERIFY(Triangle().validAvailableAngles({{0, 30}, {1, 90}, {2, 60}}));
}

void test_triangle::test_3fronts_calculating()
{
    // Test arbirary triangles
    QCOMPARE(
        Triangle({{0, 3}, {1, 4}, {2, 5}}, {}).anglesAsMap().values(),
        QList<double>({37, 53, 90})
    );
    QCOMPARE(
        Triangle({{0, 37}, {1, 40}, {2, 54}}, {}).anglesAsMap().values(),
        QList<double>({43, 48, 89})
    );

    // Test equilateral triangle
    QCOMPARE(Triangle({{0, 10}, {1, 10}, {2, 10}}, {}).anglesAsMap().values(),
             QList<double>({60, 60, 60}));

    // Test unvalid cases (some angle doens't match or 2 angles >= 90)
    QCOMPARE(Triangle({{0, 10}, {1, 10}, {2, 10}}, {{0, 90}, {1, 90}}).anglesAsMap().values(),
             QList<double>({90, 90}));

    QCOMPARE(Triangle({{0, 10}, {1, 10}, {2, 10}}, {{0, 90}}).anglesAsMap().values(),
             QList<double>({90}));
}

void test_triangle::test_angles_filling()
{
    // correct triangles
    QCOMPARE(Triangle({{0, 8}, {1, 8}, {2, 8}}, {{0, 60}, {2, 60}}).anglesAsMap().values(),
             QList<double>({60, 60, 60}));
    // uncorrect angles
    QCOMPARE(Triangle({{0, 43}, {1, 44}, {2, 42}}, {{1, 90}, {2, 90}}).anglesAsMap().values(),
             QList<double>({90, 90}));
    // uncorrect fronts
    QCOMPARE(Triangle({{0, 20}, {1, 40}, {2, 20}}, {{1, 30}, {2, 90}}).anglesAsMap().values(),
             QList<double>({30, 90}));
    // with 1 known angle
    QCOMPARE(Triangle({{0, 3}, {1, 4}, {2, 5}}, {{2, 90}}).anglesAsMap().values(),
             QList<double>({37, 53, 90}));
    // test with a wrong angle
    QCOMPARE(Triangle({{0, 37}, {1, 40}, {2, 54}}, {{0, 48}}).anglesAsMap().values(),
             QList<double>({48}));
}

void test_triangle::test_2fronts_triangle()
{
    // fronts: 89, (73), 85
    // angles: 68, 50, 62
    QCOMPARE(Triangle({{0, 89}, {2, 85}}, {{0, 68.07}, {1, 49.55}, {2, 62.38}}, 2).frontsAsMap().values(),
             QList<double>({89, 73.01, 85}));
    // fronts: (3), 4, 5
    // angles: 37, 53, 90
    QCOMPARE(Triangle({{1, 4}, {2, 5}}, {{0, 37}, {1, 53}, {2, 90}}).frontsAsMap().values(),
             QList<double>({3, 4, 5}));
    // fronts: 8, 9, (10)
    // angles: 49, 59, 72
    QCOMPARE(Triangle({{0, 8}, {1, 9}}, {{0, 49}, {1, 59}, {2, 72}}, 2).frontsAsMap().values(),
            QList<double>({8, 9, 10.03}));
    // fronts: 73, 80, 82
    QCOMPARE(Triangle({{1, 80}, {2, 82}}, {{0, 53.6}, {1, 61.8}, {2, 64.6}}, 2).frontsAsMap().values(),
             QList<double>({73.06, 80, 82}));
}

QTEST_APPLESS_MAIN(test_triangle)

#include "tst_triangle.moc"
