#include "Converter.h"
#include "Button.h"
#include "Utils.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QtWidgets>

#include <iostream>
#include <bitset>
#include <sstream>

namespace
{
int MaxDecimalLength{7};
int MaxFractionalLength{7};
int MaxBinaryLength{40};
int MaxWidth{10};
}

CConverter::CConverter(QWidget* Parent)
    : QWidget(Parent) // When inherting from QWidget, pass parent argument in constructor to the base class's constructor
{
    // Create and set the integral display 1
    QLabel* Display1Label = new QLabel(QString("Decimal"));
    m_Display1 = new QLineEdit("0");
    m_Display1->setAlignment(Qt::AlignRight);
    m_Display1->setMaxLength(MaxDecimalLength);
    m_Display1->setMaximumWidth(MaxWidth*MaxDecimalLength);
    QFont Font1 = m_Display1->font();
    Font1.setPointSize(Font1.pointSize() + 1);
    m_Display1->setFont(Font1);
    Display1Label->setFont(Font1);

    // Create and set the fractional display 1
    QLabel* Display1Label_Frac = new QLabel(QString("Fraction"));
    m_Display1_Frac = new QLineEdit("0");
    m_Display1_Frac->setAlignment(Qt::AlignLeft);
    m_Display1_Frac->setMaxLength(MaxFractionalLength);
    m_Display1_Frac->setMaximumWidth(MaxWidth*MaxFractionalLength);
    QFont Font1_Frac = m_Display1_Frac->font();
    Font1_Frac.setPointSize(Font1_Frac.pointSize() + 1);
    m_Display1_Frac->setFont(Font1_Frac);
    Display1Label_Frac->setFont(Font1_Frac);

    // Create and set the display 2
    QLabel* Display2Label = new QLabel(QString("Binary"));
    m_Display2 = new QLineEdit("0 00000000 00000000000000000000000");
    m_Display2->setReadOnly(true);
    m_Display2->setAlignment(Qt::AlignRight);
    m_Display2->setMaxLength(MaxBinaryLength);
    m_Display2->setMaximumWidth(MaxWidth*MaxBinaryLength);
    QFont Font2 = m_Display2->font();
    Font2.setPointSize(Font2.pointSize() + 1);
    m_Display2->setFont(Font2);
    Display2Label->setFont(Font2);

    QLabel* Note = new QLabel(QString("Note: clear fractional part for non-floating representation"));

    // Create button
    m_DecimalToBinary = CreateButton(tr("="), SLOT(ButtonClicked()));

    // Create dot label
    QLabel* Dot = new QLabel(QString("."));

    // Create main layout and arrange buttons
    QGridLayout* MainLayout = new QGridLayout;
    MainLayout->setSizeConstraint(QLayout::SetFixedSize);

    // Add integral display 1 to the main layout
    // Pass row, column, row span and column span
    MainLayout->addWidget(Display1Label, 0, 0, 1, 3);
    MainLayout->addWidget(m_Display1,    1, 0, 1, 3);

    // Add dot button to the main layout
    // Pass row, column, row span and column span
    MainLayout->addWidget(Dot, 1, 3, 1, 1);

    // Add fractional display 1 to the main layout
    // Pass row, column, row span and column span
    MainLayout->addWidget(Display1Label_Frac, 0, 4, 1, 3);
    MainLayout->addWidget(m_Display1_Frac,    1, 4, 1, 3);

    // Add DecimalToBinary button to the main layout
    // Pass row, column, row span and column span
    MainLayout->addWidget(m_DecimalToBinary, 1, 7, 1, 1);

    // Add display 2 to the main layout
    // Pass row, column, row span and column span
    MainLayout->addWidget(Display2Label, 0, 8, 1, 3);
    MainLayout->addWidget(m_Display2,    1, 8, 1, 3);

    // Add Note
    MainLayout->addWidget(Note, 2, 0, 1, 11);

    // Set the CConverter widget layout to main layout
    setLayout(MainLayout);
    setWindowTitle(tr("Converter"));
}

// Pressing one of the buttons will emit the button's clicked() signal which will trigger the ButtonClicked() slot.
void CConverter::ButtonClicked()
{
    std::string FractionalPart = m_Display1_Frac->text().toStdString();

    m_Display2->clear();

    if(FractionalPart.empty())
    {
        m_Display2->setText(char2binary(m_Display1->text().toInt()).c_str());
    }
    else
    {
        std::string DecimalPart = m_Display1->text().toStdString();
        std::string Number = DecimalPart + "." + FractionalPart;
        m_Display2->setText(outbits(std::stof(Number)).c_str());
    }
}

CButton* CConverter::CreateButton(const QString& Text, const char* Member)
{
    /*
     * Signals
     * 1. Signals are member functions of QObject class or one of its subclasses (e.g., QWidget).
     * 2. Signals are emitted by an object when its internal state has changed (example clicking a QAbstractButton) in a way that may be interesting to other objects.
     * 3. Qt's widgets have many predefined signals (example QAbstractButton has a predefined "clicked()" signal).
     * 4. We can also subclass widgets to add our own signals to them.
     * 5. Signals are automatically generated by the MOC and must not be implemented in the .cpp file. They can never have return types (i.e. use void).
     *
     * Slots
     * 1. Like signals, slots are member functions of QObject class or one of its subclasses (e.g., QWidget).
     * 2. A slot is called when a signal connected to it is emitted.
     * 3. Slots are normal C++ functions and can be called normally; their only special feature is that signals can be connected to them.
     * 4. Since slots are normal member functions, they follow the normal C++ rules when called directly. However, as slots, they can be
     * invoked by any component, regardless of its access level, via a signal-slot connection. This means that a signal emitted from an
     * instance of an arbitrary class can cause a private slot to be invoked in an instance of an unrelated class.
     * 5. Qt's widgets have many pre-defined slots (example QWidget has a predefined close() slot).
     * 6. We can also subclass widgets and add our own slots so that we can handle the signals that we are interested in.
     *
     * Properties
     * 1. Signals are connected to slots via the connect call
     * 2. When a signal is emitted, the slots connected to it are usually executed immediately, just like a normal function call.
     * 3. Many signals can be connected to a single slot, and a signal can be connected to many slots as needed.
     * 4. If several slots are connected to one signal, the slots will be executed one after the other, in the order they have been connected, when the signal is emitted.
     * 5. Signals and slots are disjoint components and are loosely coupled. They are a powerful mechanism allowing component programming.
     * 6. A note about arguments: experience shows that signals and slots are more reusable if they do not use special types.
     */
    CButton* Button = new CButton(Text);
    connect(Button, SIGNAL(clicked()), this, Member);
    return Button;
}