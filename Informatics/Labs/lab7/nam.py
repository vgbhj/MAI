"""
If there are some problems - you can contact with me: gtgtr3000@gmail.com, Ilya Petrov

Global vars:
DEBUG - if true - allow you to see what changed after applying some rule
STEP_BY_STEP - if true - allow you to stop your program after every rule
DARK_THEME - if true - your terminal will be dark with white letters, else light term with black letters
PATH_TO_BOOK - this param contain path to file with rules
"""
DEBUG = True
STEP_BY_STEP = False
DARK_THEME = True

from pathlib import Path

PATH_TO_BOOK = Path('test.txt')


class RuleBook:
    class Rule:
        def __init__(self, rule_tuple):
            """
            create new rule from tuple (string with rule, id of rule (number of string))
            :param rule_tuple: tuple with rule data
            """
            data = rule_tuple[0].split('->')
            self.id = rule_tuple[1]
            self.last = data[1] == '.'
            self.repl_from = data[0]
            self.repl_to = '' if self.last else data[1]

        def is_applicable(self, string):
            return self.repl_from in string

        def is_last(self):
            return self.last

        def apply(self, in_string):
            """
            If debug is True this function print every step with colors
            :param in_string: input string for applying rule
            :return: string with applied rule
            """
            if DEBUG:
                red = '\033[31m'
                green = '\033[32m'
                std = '\033[37m' if DARK_THEME else '\033[30m'
                string = in_string
                old_string = string
                string = string.replace(self.repl_from, self.repl_to, 1)
                print(f'Rule on line {self.id + 1}: '.ljust(22, ' '), end='')
                rf_index = old_string.index(self.repl_from)
                old_string = list(old_string)
                old_string.insert(rf_index, red)
                old_string.insert(rf_index + 1 + len(self.repl_from), std)
                string = list(string)
                string.insert(rf_index, green)
                string.insert(rf_index + 1 + len(self.repl_to), std)
                print(f'{"".join(old_string).ljust(30, " ")} -> {"".join(string)}' + (' ==end' if self.last else ''))
            return in_string.replace(self.repl_from, self.repl_to, 1)
        # End class Rule

    def __init__(self, filename: Path):
        """
        Create new rulebook using path to file with rules
        :param filename: file path to rules
        """
        self.data = []
        with open(filename, mode='r', encoding='utf-8') as rules:
            normal_data = self.normalize(enumerate(map(lambda x: x.rstrip('\n'), rules.readlines())))
        for row in normal_data:
            self.data.append(self.Rule(row))

    @staticmethod
    def normalize(raw_data):
        """
        This function normalize file.
        It delete comments and extra whitespaces
        :param raw_data: list of strings
        :return: list which contains tuple like (clear string with rule | number of string with this rule)
        """
        many_string_comment_active = False
        normalize_data = []
        for i, string in raw_data:
            if not many_string_comment_active:  # if we are not in many-string comment
                if '//' in string:  # if common comment in string
                    string = ''.join(string.split())
                    string = string[:string.index('//')]
                    if string != '':
                        normalize_data.append((string, i))
                elif '/*' in string:  # this string contain start of many-string comment
                    string = ''.join(string.split())
                    string = string[:string.index('/*')]
                    if string != '':
                        normalize_data.append((string, i))
                    many_string_comment_active = True
                else:
                    string = ''.join(string.split())
                    if string != '':
                        normalize_data.append((string, i))
            else:
                if '*/' not in string:
                    continue
                string = ''.join(string.split())
                string = string[string.index('*/') + 2:]
                if string != '':
                    normalize_data.append((string, i))
                many_string_comment_active = False
        return normalize_data

    def apply(self, string):
        """
        Apply all rules to some string
        :param string: some string
        :return: result of using all rules in book
        """
        skipped = True
        while skipped:
            skipped = False
            for rule in self.data:
                if rule.is_applicable(string):
                    string = rule.apply(string)
                    if STEP_BY_STEP:
                        input("Enter anything to continue")
                    if rule.is_last():
                        return string
                    skipped = True
                    break

        return string
    # End class RuleBook


if __name__ == '__main__':
    print('\033[40m\033[37m', end='') if DARK_THEME else print('\033[47m\033[30m', end='')
    rb = RuleBook(PATH_TO_BOOK)
    print('\033[32m', rb.apply(input('Enter string: ')), end='')
    print('\033[40m\033[37m', end='') if DARK_THEME else print('\033[47m\033[30m', end='')
