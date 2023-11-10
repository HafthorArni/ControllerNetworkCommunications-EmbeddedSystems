#include <util/delay.h>
#include <unity.h>

#include <PI_controller.h>

void test_integral_increase(void)
{

    PI_controller controller(0.01,3.0);

    controller.update(60.0, 1.0);
    int i1 = controller.get_integral();
    for (int i = 0; i < 100; i++) {
        controller.update(60.0, 20.0);
    }
    int i2 = controller.get_integral();

    TEST_ASSERT_LESS_THAN_INT(i2, i1);

}


void test_integral_saturation(void)
{

    PI_controller controller(0.01,3.0);

    float actual = 1.0;
    int count = 0;

    int i0 = controller.get_integral();
    controller.update(1,0.1);

    while (count < 100) {
        controller.update(60, actual);
        count++;
        if (actual < 59) actual += 1.0;
    }
    
    int i1 = controller.get_integral();
    controller.update(59, actual);
    int i2 = controller.get_integral();

    TEST_ASSERT_EQUAL(i1,i2);
    TEST_ASSERT_NOT_EQUAL(i1,i0);
}


int main()
{
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    _delay_ms(2000);

    UNITY_BEGIN(); // IMPORTANT LINE!

    RUN_TEST(test_integral_increase);
    RUN_TEST(test_integral_saturation);

    UNITY_END(); // stop unit testing
}