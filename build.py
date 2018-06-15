from conan.packager import ConanMultiPackager
import os
import copy
import re
import platform


def get_content(path):
    with open(path, 'r') as f:
        return f.read().replace('\n', '').replace('\r', '')

def get_version():
    return get_content('conan_version')

def get_channel():
    return get_content('conan_channel')

def get_conan_vars():
    login_username = os.getenv("CONAN_LOGIN_USERNAME", "hanchon")
    username = os.getenv("CONAN_USERNAME", "hanchon")
    channel = os.getenv("CONAN_CHANNEL", get_channel())
    version = os.getenv("CONAN_VERSION", get_version())
    return login_username, username, channel, version

def get_value_from_recipe(search_string, recipe="conanfile.py"):
    with open(recipe, "r") as conanfile:
        contents = conanfile.read()
        result = re.search(search_string, contents)
    return result

def get_name_from_recipe():
    return get_value_from_recipe(r'''name\s*=\s*["'](\S*)["']''').groups()[0]

def get_user_repository(username, repository_name):
    return "https://api.bintray.com/conan/{0}/{1}".format(username.lower(), repository_name)

def get_conan_upload(username):
    repository_name = os.getenv("BIPRIM_BINTRAY_REPOSITORY", "hanchon")
    return os.getenv("CONAN_UPLOAD", get_user_repository(username, repository_name))

def get_conan_remotes(username):
    remotes = [get_conan_upload(username), "https://api.bintray.com/conan/bitprim/bitprim"]
    return remotes

def get_os():
    return platform.system().replace("Darwin", "Macos")

def get_archs():
    return ["x86_64"]

def get_builder(args=None):
    name = get_name_from_recipe()
    login_username, username, channel, version = get_conan_vars()
    reference = "{0}/{1}".format(name, version)
    upload = get_conan_upload(username)
    remotes = os.getenv("CONAN_REMOTES", get_conan_remotes(username))

    archs = get_archs()
    builder = ConanMultiPackager(
        args=args,
        username=username,
        login_username=login_username,
        channel=channel,
        reference=reference,
        upload=upload,
        remotes=remotes,
        archs=archs,
    )

    return builder, name


if __name__ == "__main__":
    builder, name = get_builder()

    builder.add_common_builds()

    filtered_builds = []
    for settings, options, env_vars, build_requires in builder.builds:
        if settings["build_type"] == "Release" \
                and (not "compiler.runtime" in settings or not settings["compiler.runtime"] == "MD"):

            opts_bch = copy.deepcopy(options)
            opts_btc = copy.deepcopy(options)
            opts_ltc = copy.deepcopy(options)

            opts_bch["*:currency"] = "BCH"
            opts_btc["*:currency"] = "BTC"
            opts_ltc["*:currency"] = "LTC"

            filtered_builds.append([settings, opts_bch, env_vars, build_requires])
            filtered_builds.append([settings, opts_btc, env_vars, build_requires])
            filtered_builds.append([settings, opts_ltc, env_vars, build_requires])


    builder.builds = filtered_builds
    builder.run()