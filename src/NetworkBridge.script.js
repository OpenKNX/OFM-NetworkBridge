function nbAddChannel(device, online, progress, context) {
    var numberOfChannels = device.getParameterByName('NTB_VisibleChannels');
    var newNumberOfChannels = numberOfChannels.value + 1;
    var channelType = device.getParameterByName('NTB_CH' + newNumberOfChannels + 'ChannelType');
    var notAllowed = device.getParameterByName('NTB_AddNotAllowed');

    if (channelType.value == 0) {
        notAllowed.value = 1;
    } else {
        notAllowed.value = 0;
        numberOfChannels.value = newNumberOfChannels;
    }
}
