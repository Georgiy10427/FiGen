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
    void test_3fronts_calculating();

};

test_triangle::test_triangle()
{

}

test_triangle::~test_triangle()
{

}

void test_triangle::test_3fronts_calculating()
{
    // Test arbirary triangles
    QCOMPARE(
        Triangle({{0, 3}, {1, 4}, {2, 5}}, {}).anglesAsMap().values(),
        QList<double>({37, 53, 90})
    );
    QVERIFY(
        Triangle(
            {{0, 37}, {1, 40}, {2, 54}},
            {}
        ).anglesAsMap().values()
        ==
        QList<double>({43, 48, 89})
    );

    // Test equilateral triangle
    QCOMPARE(Triangle({{0, 10}, {1, 10}, {2, 10}}, {}).anglesAsMap().values(),
             QList<double>({60, 60, 60}));
}

QTEST_APPLESS_MAIN(test_triangle)

#include "tst_triangle.moc"
