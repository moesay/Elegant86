#ifndef HIGHLITER_H
#define HIGHLITER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>
#include <QPlainTextEdit>

class Highliter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
        Highliter(QTextDocument *parent = nullptr);
        ~Highliter() override {}
protected:
        void highlightBlock(const QString&) override;
private:
        struct st_HighlightRule {
            QRegularExpression pattern;
            QTextCharFormat format;
        };

        QVector<st_HighlightRule> hRulesVec;

        QTextCharFormat mnemonicsFmt;
        QTextCharFormat reservedWordsFmt;
        QTextCharFormat regsFmt;
        QTextCharFormat commentsFmt;
        QTextCharFormat stringsFmt;
        QTextCharFormat numbersFmt;
};

#endif // HIGHLITER_H
