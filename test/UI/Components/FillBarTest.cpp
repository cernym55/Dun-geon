#define BOOST_TEST_MODULE UI.Components.FillBar
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "Helpers.h"
#include "UI/Components/FillBar.h"

class FillBarTest : public UI::Components::FillBar
{
public:
    FillBarTest() : UI::Components::FillBar(stdscr, 16, 0, 0, 5, 10) {}

    void SetSize(int value) { m_Size = value; }
    void SetValue(int value) { m_Value = value; }
    void SetMaxValue(int value) { m_MaxValue = value; }
    void SetTextInPercent(bool value) { m_TextInPercent = value; }

    int FilledLength() const { return FillBar::FilledLength(); }
    double Ratio() const { return FillBar::Ratio(); }
    std::string TextRepresentation() const { return FillBar::TextRepresentation(); }
};

struct TestFixture : public NcursesFixture
{
    TestFixture() {}

    FillBarTest m_FillBar;
};

BOOST_FIXTURE_TEST_CASE(FilledLength, TestFixture)
{
    // Test a generic scenario
    // Arrange
    m_FillBar.SetSize(16);
    m_FillBar.SetValue(5);
    m_FillBar.SetMaxValue(10);

    // Act
    int actual = m_FillBar.FilledLength();

    // Check that the FilledLength is exactly half of the inner bar size
    int expected = (16 - 2) / 2;
    BOOST_CHECK_EQUAL(actual, expected);

    // Test an odd size
    // Arrange
    m_FillBar.SetSize(15);
    m_FillBar.SetValue(3);
    m_FillBar.SetMaxValue(10);

    // Act
    actual = m_FillBar.FilledLength();

    // Assert
    expected = 4;
    BOOST_CHECK_EQUAL(actual, expected);


    // Test a value of zero
    // Arrange
    m_FillBar.SetValue(0);

    // Act
    actual = m_FillBar.FilledLength();

    // Assert
    expected = 0;
    BOOST_CHECK_EQUAL(actual, expected);
}

BOOST_FIXTURE_TEST_CASE(TextRepresentation, TestFixture)
{
    // Test a generic scenario - absolute values
    // Arrange
    m_FillBar.SetValue(5);
    m_FillBar.SetMaxValue(10);
    m_FillBar.SetTextInPercent(false);

    // Act
    std::string actual = m_FillBar.TextRepresentation();

    // Assert
    std::string expected = "5/10";
    BOOST_CHECK_EQUAL(actual, expected);

    // Test a generic scenario - percentage
    // Arrange
    m_FillBar.SetTextInPercent(true);

    // Act
    actual = m_FillBar.TextRepresentation();
    
    // Assert
    expected = "50%";
    BOOST_CHECK_EQUAL(actual, expected);

    // Test percentage with zero percent
    // Arrange
    m_FillBar.SetValue(0);

    // Act
    actual = m_FillBar.TextRepresentation();

    // Assert
    expected = "0%";
    BOOST_CHECK_EQUAL(actual, expected);

    // Test percentage with 100 percent
    // Arrange
    m_FillBar.SetValue(10);

    // Act
    actual = m_FillBar.TextRepresentation();

    // Assert
    expected = "100%";
    BOOST_CHECK_EQUAL(actual, expected);
}