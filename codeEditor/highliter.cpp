#include "highliter.h"

Highliter::Highliter(QTextDocument *parent) : QSyntaxHighlighter(parent) {

    st_HighlightRule rule;

    mnemonicsFmt.setForeground(Qt::darkBlue);
    mnemonicsFmt.setFontItalic(true);
    mnemonicsFmt.setFontWeight(QFont::Bold);
    const QString mnemonics[] {"\\bmov\\b", "\\badd\\b"};
    for (const QString &word : mnemonics) {
        rule.format = mnemonicsFmt;
        rule.pattern = QRegularExpression(word);
        rule.pattern.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
        hRulesVec.append(rule);
    }

    regsFmt.setForeground(Qt::darkMagenta);
    const QString regs[] {"\\bax\\b", "\\bbx\\b"};
    for (const QString &word : regs) {
        rule.format = regsFmt;
        rule.pattern = QRegularExpression(word);
        rule.pattern.setPatternOptions(QRegularExpression::CaseInsensitiveOption);

        hRulesVec.append(rule);
    }

    stringsFmt.setForeground(Qt::darkRed);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = stringsFmt;
    hRulesVec.append(rule);

    commentsFmt.setForeground(Qt::darkGreen);
    rule.format = commentsFmt;
    rule.pattern = QRegularExpression(QStringLiteral(";[^\n]*"));
    hRulesVec.append(rule);
}

void Highliter::highlightBlock(const QString& text) {
    for(const st_HighlightRule &rule : qAsConst(hRulesVec)) {
        QRegularExpressionMatchIterator matchIter = rule.pattern.globalMatch(text);
        while(matchIter.hasNext()) {
            QRegularExpressionMatch match = matchIter.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}

