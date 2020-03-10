#include <string>
#include <deque>
#include "test_runner.h"
#include <algorithm>
#include <list>
using namespace std;

class Editor {
public:
    Editor() : it(text.begin()) {};
    void Left() {
        if (it != text.begin()) {
            it--;
            //it = next(it, -1);
        }
    }
    void Right() {
        if (it != text.end()) {
            it++;
        }
    }
    void Insert(char token) {
        /*it = text.insert(it, token);
        it++;*/
        text.insert(it, token);

    }
    void Cut(size_t tokens = 1) {
        if (tokens == 0)
            return;
        if (!text.empty()) {
            Copy(tokens);
            auto tempit = distance(it, end(text));
            //cout << "dist is" << tempit << "tokens is" << tokens  << endl;
            it = text.erase(it, next(it , min(tokens, static_cast<size_t>(tempit))));
        }
        //text.erase();
    }
    void Copy(size_t tokens = 1) {
        if (tokens == 0)
            return;
        //deque<char>::iterator tempit;
        auto tempit = distance(it, end(text));
        buffer = { it, next(it, min(tokens, static_cast<size_t>(tempit))) };
    }
    void Paste() {
        if (!buffer.empty()) {
            text.insert(it, begin(buffer), end(buffer));
            //it = next(it, buffer.size());
        }
    }
    string GetText() const {
        /*string s;
        for (const auto& i : text) {
            s += i;
        }
        return s;*/
        return {begin(text), end(text)};
    }
private:
    /*deque<char> text;
    deque<char>::iterator it;
    deque<char> buffer;*/
    list<char> text;
    list<char>::iterator it;
    list<char> buffer;
    //pair<list<char>::iterator, list<char>::iterator> buffer;
};

void TypeText(Editor& editor, const string& text) {
    for (char c : text) {
        editor.Insert(c);
    }
}

void TestEditing() {
    {
        Editor editor;

        const size_t text_len = 12;
        const size_t first_part_len = 7;
        TypeText(editor, "hello, world");//hello, world
        for (size_t i = 0; i < text_len; ++i) {
            editor.Left();
        }
        editor.Cut(first_part_len);
        for (size_t i = 0; i < text_len - first_part_len; ++i) {
            editor.Right();
        }
        TypeText(editor, ", ");
        editor.Paste();
        editor.Left();
        editor.Left();
        editor.Cut(3);

        ASSERT_EQUAL(editor.GetText(), "world, hello");
    }
    {
        Editor editor;

        TypeText(editor, "misprnit");
        editor.Left();
        editor.Left();
        editor.Left();
        editor.Cut(1);
        editor.Right();
        editor.Paste();

        ASSERT_EQUAL(editor.GetText(), "misprint");
    }
}

void TestReverse() {
    Editor editor;

    const string text = "esreveR";
    for (char c : text) {
        editor.Insert(c);
        editor.Left();
    }

    ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
    Editor editor;
    ASSERT_EQUAL(editor.GetText(), "");

    editor.Left();
    editor.Left();
    editor.Right();
    editor.Right();
    editor.Copy(0);
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
    Editor editor;

    editor.Paste();
    TypeText(editor, "example"); //example
    editor.Left();
    editor.Left();
    editor.Paste();
    editor.Right();
    editor.Paste();
    editor.Copy(0);
    editor.Paste();
    editor.Left();
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestEditing);
    RUN_TEST(tr, TestReverse);
    RUN_TEST(tr, TestNoText);
    RUN_TEST(tr, TestEmptyBuffer);
    return 0;
}