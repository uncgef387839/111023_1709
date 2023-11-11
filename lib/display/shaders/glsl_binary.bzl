# Used a helpful example from here for reference: https://github.com/RenatoUtsch/rules_spirv/tree/master

def _glsl_binary_impl(ctx):
    binary = ctx.outputs.binary

    cmd = "glslc " + ctx.file.src.path + " -o " + binary.path

    ctx.actions.run_shell(
        inputs = [ctx.file.src],
        outputs = [binary],
        command = cmd,
        mnemonic = "glslc",
        progress_message = "Compiling SPIR-V binary {}".format(binary.short_path),
    )

    runfiles = ctx.runfiles(
        files = [binary],
    )

    return [DefaultInfo(runfiles = runfiles)]

glsl_binary = rule(
    attrs = {
        "src": attr.label(
            allow_single_file = True,
            mandatory = True,
        ),
    },
    outputs = {
        "binary": "%{name}.spv",
    },
    implementation = _glsl_binary_impl,
)
