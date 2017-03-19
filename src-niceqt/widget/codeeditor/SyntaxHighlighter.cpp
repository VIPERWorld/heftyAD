#include "SyntaxHighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
}

void SyntaxHighlighter::registerRule(const SyntaxHighlighter::SyntaxHighlightingRule &rule)
{
    m_syntaxHighlightingRules.append(rule);
}

void SyntaxHighlighter::registerRule(const QTextCharFormat &format, const QRegExp &expression)
{
    registerRule({format, expression});
}

void SyntaxHighlighter::registerRule(const QTextCharFormat &format, const QString &pattern)
{
    registerRule(format, QRegExp(pattern));
}

void SyntaxHighlighter::registerRules(const QTextCharFormat &format, const QList<QRegExp> &expressions)
{
    for(const auto &expr : expressions) {
        registerRule(format, expr);
    }
}

void SyntaxHighlighter::registerRules(const QTextCharFormat &format, const QStringList &patterns)
{
    for(const auto &pattern : patterns) {
        registerRule(format, pattern);
    }
}

void SyntaxHighlighter::clearRules()
{
    m_syntaxHighlightingRules.clear();
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    for(const SyntaxHighlightingRule &rule : m_syntaxHighlightingRules) {
        highlightBlock(text, rule);
    }
}

void SyntaxHighlighter::highlightBlock(const QString &text, const SyntaxHighlighter::SyntaxHighlightingRule &rule)
{
    const QTextCharFormat &format(rule.format);
    const QString &pattern(rule.expression.pattern()); // if expression is used directly, the program stucks.

    QRegExp expression(pattern);
    int index = text.indexOf(expression);
    while(index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, format);
        index = text.indexOf(expression, index + length);
    }
}
