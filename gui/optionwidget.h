#ifndef OptionWidget_H
#define OptionWidget_H

#include <QWidget>
#include <QPainter>
#include <QFontMetrics>
#include <QDomElement>

#include <QDebug>

#define CENTRAL_PADDING 15

class OptionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OptionWidget(QWidget *parent = 0);
    QList<QDomElement> options() const;
    void setOptions(const QList<QDomElement> &options);
    void selectNextOption();
    void selectPreviousOption();

    void onEnter();
    void onRight();
    void onLeft();
    void onBackspace();

    bool isApplySelected() const;
    bool isCancelSelected() const;
    bool isSelectionLocked() const;
    void selectCancelButton();
    void lockSelection();

    static QString optionValue(const QDomElement &option);

protected:
    void paintEvent(QPaintEvent *);

    int paintOptionName(QPainter *painter, const QString &option, bool selected);
    void paintRect(QPainter *painter, const QRect &rect);
    void paintCheckbox(QPainter *painter, const QDomElement &option, bool selected);
    void paintLine(QPainter *painter, const QDomElement &option, bool selected);
    void paintSpin(QPainter *painter, const QDomElement &option, bool selected);

    void paintButtons(QPainter *painter);

    void changeLineChar(int delta);
    void moveCursor(int delta);
    void moveSpin(int delta);
    void removeChar();
    bool isProfileCorrect() const;

signals:
    void spinChanged(const QString &name, int value);

private:
    int m_currentLine;
    int m_selectedLine;
    int m_isSelectionLocked;
    int m_cursor;
    QList<QDomElement> m_options;
};

#endif // OptionWidget_H
