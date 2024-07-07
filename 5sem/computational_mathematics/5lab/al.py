def find_entries_by_key_and_value(list_of_entries, key, value):
    new_list_of_entries = []

    for entry in list_of_entries:
        if entry.get(key) is not None and entry.get(key) == value:
            new_list_of_entries.append(entry)

    return new_list_of_entries


list_of_entries = [
    {
        'id': 1,
        'title': 'a',
        'price': 100,
        'count': 4
    },
    {
        'id': 2,
        'title': 'b',
        'price': 10,
        'count': 3
    },
    {
        'id': 3,
        'title': 'c',
        'price': 43,
        'count': 93
    },
    {
        'id': 4,
        'title': 'd',
        'price': 2412,
        'count': 12
    },
    {
        'id': 5,
        'title': 'e',
        'price': 10,
        'count': 15
    }
]

new_list_of_entries = find_entries_by_key_and_value(list_of_entries, 'price', 10)

print(new_list_of_entries)
