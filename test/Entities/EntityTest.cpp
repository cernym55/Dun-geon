#define BOOST_TEST_MODULE Entities.Entity
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "Entities/Entity.h"

BOOST_AUTO_TEST_CASE(Constructor)
{
    // Check that the default constructor assigns the correct values
    Entities::Entity defaultEntity("TestName");
    BOOST_CHECK_EQUAL(defaultEntity.GetName(), "TestName");
    BOOST_CHECK_EQUAL(defaultEntity.GetDescription().size(), 0);
    BOOST_CHECK_EQUAL(defaultEntity.IsBlocking(), true);
    // Check that the icon defaults to the first character of the name
    BOOST_CHECK_EQUAL(defaultEntity.GetIcon(), 'T');

    // Check that the icon is left as is when specified explicitly
    Entities::Entity nonDefaultEntity("TestName", "TestDescription", 'A', false);
    BOOST_CHECK_EQUAL(nonDefaultEntity.GetIcon(), 'A');
}
