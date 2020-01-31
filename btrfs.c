#include <fcntl.h>
#include <sys/ioctl.h>

#include <btrfs/ioctl.h>
#include <btrfs/ctree.h>

#include <python3.6m/Python.h>

PyObject *get_qgroups(const char *path)
{
    PyObject *qgroups = PyList_New(0);

    PyObject *result = PyTuple_New(2);

    PyTuple_SetItem(result, 1, qgroups);

    struct btrfs_ioctl_search_args search_args;
    struct btrfs_ioctl_search_key *search_key = &search_args.key;

    memset(&search_args, 0, sizeof(search_args));

    search_key->tree_id = BTRFS_QUOTA_TREE_OBJECTID;
    search_key->max_type = BTRFS_QGROUP_INFO_KEY;
    search_key->min_type = BTRFS_QGROUP_INFO_KEY;
    search_key->max_objectid = (u64) -1;
    search_key->max_offset = (u64) -1;
    search_key->max_transid = (u64) -1;
    search_key->nr_items = 4096;

    int fd = open(path, O_RDONLY);

    while (true)
    {
        int return_code = ioctl(fd, BTRFS_IOC_TREE_SEARCH, &search_args);

        if (return_code < 0)
        {
            PyTuple_SetItem(result, 0, PyLong_FromLong(-(errno == ENOENT ? ENOTTY : errno)));

            return result;
        }

        if (search_key->nr_items == 0)
        {
            break;
        }

        size_t offset = 0;

        for (u32 i = 0; i < search_key->nr_items; ++i)
        {
            struct btrfs_ioctl_search_header *search_header = (struct btrfs_ioctl_search_header *) (search_args.buf +
                                                                                                    offset);

            if (btrfs_search_header_type(search_header) == BTRFS_QGROUP_INFO_KEY)
            {
                u64 qgroup_id = btrfs_search_header_offset(search_header);

                PyObject *qgroup = PyTuple_New(2);

                PyTuple_SetItem(qgroup, 0, PyLong_FromLong(btrfs_qgroup_level(qgroup_id)));
                PyTuple_SetItem(qgroup, 1, PyLong_FromLong(btrfs_qgroup_subvid(qgroup_id)));

                PyList_Append(qgroups, qgroup);

                search_key->min_offset = btrfs_search_header_offset(search_header);
                search_key->min_objectid = btrfs_search_header_objectid(search_header);
            }
            else
            {
                search_key->min_type = btrfs_search_header_type(search_header);
                search_key->min_offset = btrfs_search_header_offset(search_header);
                search_key->min_objectid = btrfs_search_header_objectid(search_header);
            }

            offset += sizeof(*search_header) + btrfs_search_header_len(search_header);
        }

        search_key->nr_items = 4096;

        if (search_key->min_offset < (u64) -1)
        {
            search_key->min_offset++;
        }
        else
        {
            break;
        }
    }

    close(fd);

    PyTuple_SetItem(result, 0, PyLong_FromLong(0));

    return result;
}
